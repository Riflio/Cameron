#include "serverclientstreamer.h"

#include <QDebug>
#include "rtp/rtppacketh264.h"
#include "rtp/rtppacketh264unitfu.h"
#include "rtp/rtppacketh265.h"
#include "rtp/rtppacketh265unitfu.h"

/**
* @brief Server_Client_Streamer::Server_Client_Streamer
* @param parent
* @param host
* @param port
* @param id - айдишник. Соответствует камере.
* @param streamer
*/
ServerClientStreamer::ServerClientStreamer(QObject *parent, QHostAddress host, int port, int id, IRTSPStream *streamer): QObject(parent), _host(host), _port(port), _id(id), _streamer(streamer)
{

}

int ServerClientStreamer::id()
{
  return _id;
}

/**
* @brief Отдаём максимально возможный размер пакета, что бы влез в MTU
* @return
*/
uint32_t ServerClientStreamer::blockSize()
{
  return _blockSize;
}

/**
* @brief Задаём максимально возможный размер пакета, что бы влез в MTU
* @param blockSize
*/
void ServerClientStreamer::setBlockSize(uint32_t blockSize)
{
  _blockSize =qMax(200U, blockSize);
}

/**
* @brief Есть новый пакет, отправляем клиенту
*/
void ServerClientStreamer::onNewPacketAvaliable(QSharedPointer<IRTPPacket> packet)
{
  if ( packet->data().isEmpty() ) { return; }

  //-- Если разрмер пакета превышает blockSize, то фрагментируем
  if ( packet->size()>_blockSize ) {
    switch ( packet->payloadType() ) {
      case IRTPPacket::PT_H265: { //-- h265
        RTPPacketH265 *rtph265 =dynamic_cast<RTPPacketH265*>(packet.data());

        if ( rtph265->isFUUnit() ) { //-- Фрагментируем FU пакет на несколько, что бы влезли в BlockSize
          RTPPacketH265UnitFU *rtph265FU =dynamic_cast<RTPPacketH265UnitFU*>(rtph265);

          //-- Будем фрагментировать копию, что бы исходный не задеть, т.к. он может быть нужен другим в другом размере //TODO: Возможно имеет смысл фрагментировать не здесь, а при получении по минимальному BlockSize
          RTPPacketH265UnitFU partRTPh265FU(QByteArray(rtph265FU->data().data(), rtph265->size()));

          const RTPPacketH265UnitFU::FU_PARTS srcFUPart =rtph265FU->fuPart(); //-- Запоминаем какая часть была изначально, т.к. мы будет добавлять новые нужно сохранить правильный порядок

          const uint8_t *packetContent =partRTPh265FU.rawData();
          const uint64_t packetSize =partRTPh265FU.size();
          const uint16_t commonHeaderSize =partRTPh265FU.payloadStart(); //-- Выясняем размер заголовка, т.к. он должен быть в каждом пакете
          const uint64_t packetDataSize =partRTPh265FU.payloadLength(); //-- Выясняем размер данных

          const uint64_t optimalPartSize =_blockSize-commonHeaderSize; //-- Выясняем оптимальный размер одного фрагмента с учётом, что в каждом пакете будет общий заголовок
          const uint32_t parts =uint16_t(packetDataSize/optimalPartSize)+1;
          uint64_t partSize =uint32_t(packetDataSize/parts)+1; //-- Размер одного фрагмента

          //-- Отправляем пакеты
          uint64_t partContentOffset =commonHeaderSize;
          while ( partContentOffset<packetSize) {
            //-- В заголовке прописываем нужные параметры
            partRTPh265FU.setSequence(++_sequence);
            //-- Если исходный пакет был начальным, то первый фрагмент то же должен остаться начальным
            if ( partContentOffset==commonHeaderSize && srcFUPart==RTPPacketH265UnitFU::FU_START ) { partRTPh265FU.setFUPart(RTPPacketH265UnitFU::FU_START); }
            //-- Если исходный пакет был конечным, то последний фрагмент то же должен быть конечным
            else if ( partContentOffset+partSize>=packetSize && srcFUPart==RTPPacketH265UnitFU::FU_END ) { partRTPh265FU.setFUPart(RTPPacketH265UnitFU::FU_END);  }
            //-- Остальные фрагменты промежуточными быть должны
            else { partRTPh265FU.setFUPart(RTPPacketH265UnitFU::FU_MIDDLE); }
            //-- Копируем заголовок перед данными
            std::memcpy((char *)(packetContent+partContentOffset-commonHeaderSize), packetContent, commonHeaderSize);
            //-- Отправляем
            partSize =qMin(partSize, packetSize-partContentOffset); //-- Учитываем, что последний фрагмент может быть меньше по размеру
            _socket->writeDatagram((const char *)(packetContent+partContentOffset-commonHeaderSize), partSize+commonHeaderSize, _host, _port);
            //-- Идём дальше
            partContentOffset +=partSize;
          }

          return; //-- Всё ок, отправили
        }
        //-- Остальное пока что не знаем как фрагментировать, отправляем как есть
        break;
      }
      case IRTPPacket::PT_H264: { //-- h264
        RTPPacketH264 *rtph264 =dynamic_cast<RTPPacketH264*>(packet.data());

        //-- Фрагментируем FU пакет на несколько, что бы влезли в blockSize
        if ( rtph264->isFUUnit() ) {
          RTPPacketH264UnitFU *rtph264FU =dynamic_cast<RTPPacketH264UnitFU*>(rtph264);

          //-- Будем фрагментировать копию, что бы исходный не задеть, т.к. он может быть нужен другим в другом размере //TODO: Возможно имеет смысл фрагментировать не здесь, а при получении по минимальному BlockSize
          RTPPacketH264UnitFU partRTPh264FU(QByteArray(rtph264FU->data().data(), rtph264->size()));

          const RTPPacketH264UnitFU::FU_PARTS srcFUPart =rtph264FU->fuPart(); //-- Запоминаем какая часть была изначально, т.к. мы будет добавлять новые нужно сохранить правильный порядок
          const uint8_t *packetContent =partRTPh264FU.rawData();
          const uint64_t packetSize =partRTPh264FU.size();
          const uint16_t commonHeaderSize =partRTPh264FU.payloadStart(); //-- Выясняем размер заголовка, т.к. он должен быть в каждом пакете
          const uint64_t packetDataSize =partRTPh264FU.payloadLength(); //-- Выясняем размер данных

          const uint64_t optimalPartSize =_blockSize-commonHeaderSize; //-- Выясняем оптимальный размер одного фрагмента с учётом, что в каждом пакете будет общий заголовок
          const uint32_t parts =uint16_t(packetDataSize/optimalPartSize)+1;
          uint64_t partSize =uint32_t(packetDataSize/parts)+1; //-- Размер одного фрагмента

          //-- Отправляем пакеты
          uint64_t partContentOffset =commonHeaderSize;
          while ( partContentOffset<packetSize) {
            //-- В заголовке прописываем нужные параметры
            partRTPh264FU.setSequence(++_sequence);
            //-- Если исходный пакет был начальным, то первый фрагмент то же должен остаться начальным
            if ( partContentOffset==commonHeaderSize && srcFUPart==RTPPacketH264UnitFU::FU_START ) { partRTPh264FU.setFUPart(RTPPacketH264UnitFU::FU_START); }
            //-- Если исходный пакет был конечным, то последний фрагмент то же должен быть конечным
            else if ( partContentOffset+partSize>=packetSize && srcFUPart==RTPPacketH264UnitFU::FU_END ) { partRTPh264FU.setFUPart(RTPPacketH264UnitFU::FU_END);  }
            //-- Остальные фрагменты промежуточными быть должны
            else { partRTPh264FU.setFUPart(RTPPacketH264UnitFU::FU_MIDDLE); }
            //-- Копируем заголовок перед данными
            std::memcpy((char *)(packetContent+partContentOffset-commonHeaderSize), packetContent, commonHeaderSize);
            //-- Отправляем
            partSize =qMin(partSize, packetSize-partContentOffset); //-- Учитываем, что последний фрагмент может быть меньше по размеру
            _socket->writeDatagram((const char *)(packetContent+partContentOffset-commonHeaderSize), partSize+commonHeaderSize, _host, _port);
            //-- Идём дальше
            partContentOffset +=partSize;
          }

          return; //-- Всё ок, отправили
        }
        //-- Остальное пока что не знаем как фрагментировать, отправляем как есть
        break;
      }

      default: break;
    }
  }

  //-- Отправляем целиком если не нуждается или не удалось фрагментировать
  RTPPacket newRTPPacket(QByteArray(packet->data().data(), packet->size()));
  newRTPPacket.setSequence(++_sequence);
  qint64 dataSendedSize =_socket->writeDatagram(newRTPPacket.data(), _host, _port);
  if ( dataSendedSize<0 ) {}

}

void ServerClientStreamer::start()
{
  if ( _socket==nullptr ) { _socket =new QUdpSocket(this); }
  connect(dynamic_cast<RTSPStream*>(_streamer), &RTSPStream::newPacketAvaliable, this, &ServerClientStreamer::onNewPacketAvaliable, Qt::QueuedConnection);
}

ServerClientStreamer::~ServerClientStreamer()
{
  qDebug()<<"";
}
