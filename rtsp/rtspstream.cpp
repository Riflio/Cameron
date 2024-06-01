#include "rtspstream.h"
#include "rtp/rtppacket.h"
#include "rtp/rtppacketh264unitfu.h"
#include "rtp/rtppacketh265unitfu.h"
#include <QDebug>

namespace NS_RSTP {

RTSPStream::RTSPStream(QObject *parent): IQRTSPStream(parent)
{

}

RTSPStream::~RTSPStream()
{
  if ( _socket!=nullptr && _socket->isOpen() ) { _socket->close(); }
}

/**
* @brief Сообщаем о возникшей ошибке
*/
void RTSPStream::goError()
{
  emit errored();
}

/**
* @brief Есть новый фрейм
*/
void RTSPStream::onReadyRead()
{
  while ( _socket->hasPendingDatagrams() ) {
    QByteArray ba(_socket->pendingDatagramSize(), Qt::Uninitialized);
    _socket->readDatagram(ba.data(), ba.size());

    //-- В зависимости от типа создадим соответствующий класс, возьмём мета-информацию, если в пакете она
    IRTPPacket *packet =nullptr;
    switch (RTPPacket::getPayloadType(ba)) {
      case RTPPacket::PT_H264: {
        RTPPacketH264UnitFU *packetH264FU =new RTPPacketH264UnitFU(std::move(ba));
        if ( packetH264FU->isFUUnit() ) {
          //-- Соберём мета-информацию, если в пакете она
          switch (packetH264FU->nalType()) {
            case RTPPacketH264UnitFU::FU_A:
            case RTPPacketH264UnitFU::FU_B: {
              switch (packetH264FU->fuType()) {
                case IRTPPacketH264UnitFU::IDR: {
                  QByteArray &miba =_metaInfo[packetH264FU->fuType()+IRTPPacketH264UnitFU::MAX]; //-- Придётся временно запомнить, пока полностью не соберём
                  switch (packetH264FU->fuPart()) {
                    case IRTPPacketH264UnitFU::FU_START: {
                      miba.clear();
                      miba.append(packetH264FU->nalHeader());
                      miba.append((const char *)packetH264FU->payloadData(), packetH264FU->payloadType());
                      break;
                    }
                    case IRTPPacketH264UnitFU::FU_MIDDLE: {
                      miba.append((const char *)packetH264FU->payloadData(), packetH264FU->payloadType());
                      break;
                    }
                    case IRTPPacketH264UnitFU::FU_END: {
                      miba.append((const char *)packetH264FU->payloadData(), packetH264FU->payloadType());
                      _metaInfo[packetH264FU->fuType()] =miba;
                      break;
                    }
                    default: {
                      break;
                    }
                  }
                  break;
                }
                default: {
                  break;
                }
              }
              break;
            }
            default: {
              break;
            }
          }
          packet =packetH264FU;
        } else {
          RTPPacketH264 *packetH264 =static_cast<RTPPacketH264 *>(packetH264FU);
          //-- Соберём мета-информацию, если в пакете она
          switch (packetH264->nalType()) {
            case IRTPPacketH264::SPS:
            case IRTPPacketH264::PPS:
            case IRTPPacketH264::SEI:
            case IRTPPacketH264::IDR: {
              RTPPacket *packetRTP =static_cast<RTPPacket *>(packetH264); //-- Нужно будет запомнить и заголовок H264 фрейма SPS/PPS/IDR
              _metaInfo[packetH264->nalType()] =QByteArray((const char *)packetRTP->payloadData(), packetRTP->payloadLength());
              break;
            }
            default: { break; }
          }
          packet =packetH264;
        }
        break;
      }
      case RTPPacket::PT_H265: {
        static RTPPacketH265 packetH265Temp(ba);
        packetH265Temp.setData(ba);

        if ( packetH265Temp.isFUUnit() ) {
          RTPPacketH265UnitFU *packetH265FU =new RTPPacketH265UnitFU(std::move(ba));
          packet =packetH265FU;
        } else {
          RTPPacketH265 *packeth265 =new RTPPacketH265(std::move(ba));
          switch (packeth265->nalType()) {
            case IRTPPacketH265::SPS:
            case IRTPPacketH265::PPS: {
              RTPPacket *packetRTP =static_cast<RTPPacket *>(packeth265); //-- Нужно будет запомнить и заголовок H265 фрейма SPS/PPS/IDR
              _metaInfo[packeth265->nalType()] =QByteArray((const char *)packetRTP->payloadData(), packetRTP->payloadLength());
              break;
            }
            default: { break; }
          }
          packet =packeth265;
        }
        break;
      }
      default: { //-- Не знаем больше конкретных, отправляем как есть
        packet =new RTPPacket(std::move(ba));
        break;
      }
    }
    if ( packet==nullptr ) { return; }
    emit newPacketAvaliable(QSharedPointer<IRTPPacket>(packet));
  }
}

/**
* @brief Запускают поток
*/
void RTSPStream::start()
{
  if ( _socket==nullptr ) {
    _socket =new QUdpSocket(this);
    connect(_socket, &QUdpSocket::connected, this, &RTSPStream::connected);
    connect(_socket, &QUdpSocket::disconnected, this, &RTSPStream::disconnected);
    connect(_socket, &QUdpSocket::readyRead, this, &RTSPStream::onReadyRead);
  }

  if ( !_socket->bind(_port) ) { //-- По каким-то причинам не удалось создать подключение, генерируем ошибку
    qWarning()<<"Unable bind rtsp_stream socket!";
    goError();
  }

}

/**
* @brief RTSPStream::setPort
* @param port
*/
void RTSPStream::setPort(int port)
{
  _port =port;
}

/**
* @brief Информация, необходимая для начала использования данных со стримера. Конкретные значения зависият от типа содержимого потока. @see onReadyRead()
* @return
*/
const QMap<uint16_t, QByteArray> &RTSPStream::metaInfo() const
{
  return _metaInfo;
}

/**
* @brief Устанавливаем полностью новую информацию, необходимую для начала использования данных со стримера.
* @param newMetaInfo
* @return
*/
bool RTSPStream::setMetaInfo(const QMap<uint16_t, QByteArray> &newMetaInfo)
{
  _metaInfo =newMetaInfo;
  return true;
}



}
