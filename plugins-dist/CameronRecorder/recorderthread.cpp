#include "recorderthread.h"
#include "interfaces/irtppacketh264unitfu.h"
#include <QDebug>

RecorderThread::RecorderThread(): QObject(nullptr)
{

}

RecorderThread::~RecorderThread()
{

}

/**
* @brief Задают откуда брать кадры
* @param streamer
*/
bool RecorderThread::setStreamer(NS_RSTP::IRTSPStream *streamer)
{
  if ( streamer==nullptr ) { return false; }
  _streamer =streamer;

  return true;
}

/**
* @brief Задают новый файл для записи
* @param recFileInfo
* @return
*/
bool RecorderThread::setRecFileInfo(TRecFileInfo *recFileInfo)
{
  if ( recFileInfo==nullptr ) { return false; }
  if ( _recFileInfo==recFileInfo ) { return true; }

  _busy =true;
  _recFileInfo =recFileInfo;

  //-- Откроем новый файл на запись. Если существовал, то перезапишем.
  if ( _outFile!=nullptr ) { _outFile->close(); }
  else { _outFile =new QFile(this); }

  _outFile->setFileName(_recFileInfo->path);
  if ( _outFile->open(QIODevice::WriteOnly | ( _recFileInfo->size==0? QIODevice::Truncate : QIODevice::Append)) ) { _busy =false; return false; }


  _busy =false;
  return true;
}

/**
* @brief Поток запущен
*/
void RecorderThread::onStarted()
{
  if ( _streamer==nullptr ) { emit errored(); return; }
  if ( _recFileInfo==nullptr ) { emit errored(); return; }
  _hasMetaInfo =false;
  connect(dynamic_cast<NS_RSTP::IQRTSPStream*>(_streamer), &NS_RSTP::IQRTSPStream::newPacketAvaliable, this, &RecorderThread::onNewFrameAvaliable, Qt::DirectConnection);
}

/**
* @brief Есть новый кадр от потока камеры
*/
void RecorderThread::onNewFrameAvaliable(QSharedPointer<IRTPPacket> packet)
{
  //-- Запускаем обработку в нашем потоке, если не заняты, иначе вынуждены пропустить его, что бы не задерживать очередь новых кадров
  if ( _busy ) { return; }

  if ( packet.isNull() || _outFile==nullptr || !_outFile->isOpen() ) { return; }
  QMetaObject::invokeMethod(this, "processNewFrame", Qt::QueuedConnection, Q_ARG(QSharedPointer<IRTPPacket>, packet));
}

/**
* @brief Обрабатываем кадр
* @param packet
*/
void RecorderThread::processNewFrame(QSharedPointer<IRTPPacket> packet)
{
  _busy =true;

  switch (packet->payloadType()) {
    case IRTPPacket::PT_H264: {
      IRTPPacketH264 *packH264 =dynamic_cast<IRTPPacketH264 *>(packet.data());

      //-- Первым в файле должно идти SPS, PPS, IDR, пока этого не будет неьлзя будет воспроизвести
      if ( !_hasMetaInfo ) {
        const QMap<uint16_t, QByteArray> &metaInfo =_streamer->metaInfo();
        if ( !metaInfo.contains(IRTPPacketH264::SPS) || !metaInfo.contains(IRTPPacketH264::PPS) || !metaInfo.contains(IRTPPacketH264::IDR) || metaInfo[IRTPPacketH264::IDR].size()==0 ) { break; }
        writeToOutFile(packH264->NAL_DELEMITER);
        writeToOutFile(metaInfo[IRTPPacketH264::SPS]);
        writeToOutFile(packH264->NAL_DELEMITER);
        writeToOutFile(metaInfo[IRTPPacketH264::PPS]);
        if ( metaInfo.contains(IRTPPacketH264::SEI) ) {
          writeToOutFile(packH264->NAL_DELEMITER);
          writeToOutFile(metaInfo[IRTPPacketH264::SEI]);
        }
        writeToOutFile(packH264->NAL_DELEMITER);
        writeToOutFile(metaInfo[IRTPPacketH264::IDR]);
        _hasMetaInfo =true;
        break;
      }
      //-- Сами данные фрейма
      switch (packH264->nalType()) {
        case IRTPPacketH264::FU_A:
        case IRTPPacketH264::FU_B: { //-- Фрейм разбит на несколько частей
          IRTPPacketH264UnitFU *packH264FU =dynamic_cast<IRTPPacketH264UnitFU *>(packet.data());
          switch (packH264FU->fuPart()) {
            case IRTPPacketH264UnitFU::FU_START: {
              _frame.clear();
              _frame.append(packH264FU->NAL_DELEMITER);
              _frame.append(packH264FU->nalHeader());
              _frame.append((const char *)packH264FU->payloadData(), packH264FU->payloadLength());
              break;
            }
            case IRTPPacketH264UnitFU::FU_MIDDLE: {
              _frame.append((const char *)packH264FU->payloadData(), packH264FU->payloadLength());
              break;
            }
            case IRTPPacketH264UnitFU::FU_END: {
              _frame.append((const char *)packH264FU->payloadData(), packH264FU->payloadLength());
              writeToOutFile(_frame);
              break;
            }
            default: {
              break;
            }
          }
          break;
        }
        default: { //-- Остальные типы фреймов пишем как есть
          _frame.clear();
          _frame.append(packH264->NAL_DELEMITER);
          _frame.append(packH264->nalHeader());
          _frame.append((const char *)packH264->payloadData(), packH264->payloadLength());
          writeToOutFile(_frame);
          break;
        }
      }

      break;
    }
    case IRTPPacket::PT_H265: {
      qDebug()<<"H265";
      break;
    }
    default: { qDebug()<<"UNKNOWN TYPE"<<packet->payloadType(); break; }
  }

  _busy =false;
}

/**
* @brief Пишем в файл QByteArray
* @param data
* @return
*/
bool RecorderThread::writeToOutFile(const QByteArray &data)
{
  if ( _outFile==nullptr ) { return false; }
  _outFile->write(data);
  _recFileInfo->size +=data.size();
  return true;
}

/**
* @brief Пишем в файл один байт
* @param data
* @return
*/
bool RecorderThread::writeToOutFile(uint8_t data)
{
  if ( _outFile==nullptr ) { return false; }
  _outFile->write((const char *)&data, 1);
  _recFileInfo->size +=1;
  return true;
}

/**
* @brief Пишем в файл массив байт
* @param data
* @param dataSize
* @return
*/
bool RecorderThread::writeToOutFile(uint8_t *data, size_t dataSize)
{
  if ( _outFile==nullptr || data==nullptr || dataSize==0 ) { return false; }
  _outFile->write((const char *)data, dataSize);
  _recFileInfo->size +=dataSize;
  return true;
}
