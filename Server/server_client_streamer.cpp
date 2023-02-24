#include "server_client_streamer.h"

#include <QDebug>
#include "rtp/rtp_packet_h264.h"
#include "rtp/rtp_packet_h264_unit_fu.h"

/**
* @brief Server_Client_Streamer::Server_Client_Streamer
* @param parent
* @param host
* @param port
* @param id - айдишник. Соответствует камере.
* @param streamer
*/
Server_Client_Streamer::Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id, IRTSP_Stream * streamer)
    : WThread(parent, "Client stream"), _host(host), _port(port), _id(id), _streamer(streamer)
{

}

int Server_Client_Streamer::id()
{
  return _id;
}

/**
* @brief Отдаём максимально возможный размер пакета, что бы влез в MTU
* @return
*/
uint32_t Server_Client_Streamer::blockSize()
{
  return _blockSize;
}

/**
* @brief Задаём максимально возможный размер пакета, что бы влез в MTU
* @param blockSize
*/
void Server_Client_Streamer::setBlockSize(uint32_t blockSize)
{
  _blockSize =qMax(200U, blockSize);
}

/**
* @brief Есть новый пакет
*/
void Server_Client_Streamer::onNewPacketAvaliable(QSharedPointer<IRTP_Packet> packet)
{
  if ( packet->data().isEmpty() ) { return; }

  //-- Если разрмер пакета превышает BlockSize, то фрагментируем
  if ( packet->size()>_blockSize ) {
    if ( packet->payloadType()==96 ) { //FIXME: type dynamic, depends SDP a=rtpmap:96
      RTP_Packet_H264 *rtph264 = reinterpret_cast<RTP_Packet_H264*>(packet.data());

      //-- Фрагментируем FU пакет на несколько, что бы влезли в BlockSize
      if ( !rtph264->isFUUnit()) { qWarning()<<"Frame type unsupported fragmentation"<<rtph264->nalType();  }

      RTP_Packet_H264_Unit_FU *rtph264FU = reinterpret_cast<RTP_Packet_H264_Unit_FU*>(rtph264);

      //-- Будем фрагментировать копию, что бы исходный не задеть, т.к. он может быть нужен другим в другом размере //TODO: Возможно имеет смысл фрагментировать не здесь, а при получении по минимальному BlockSize
      RTP_Packet_H264_Unit_FU partRTPh264FU(QByteArray(rtph264FU->data().data(), rtph264->size()));

      const RTP_Packet_H264_Unit_FU::FU_PARTS srcFUPart =rtph264FU->fuPart(); //-- Запоминаем какая часть была изначально, т.к. мы будет добавлять новые нужно сохранить правильный порядок
      const uint8_t *packetContent =partRTPh264FU.rawData();
      const uint64_t packetSize =partRTPh264FU.size();
      const uint16_t commonHeaderSize =partRTPh264FU.payloadStart(); //-- Выясняем размер заголовка, т.к. он должен быть в каждом пакете
      const uint64_t packetDataSize =partRTPh264FU.payloadLength(); //-- Выясняем размер данных

      const uint64_t optimalPartSize =_blockSize-commonHeaderSize; //-- Выясняем оптимальный размер одного фрагмента с учётом, что в каждом пакете будет общий заголовок
      const uint32_t parts =uint16_t(packetDataSize/optimalPartSize)+1;
      uint64_t partSize =uint32_t(packetDataSize/parts)+1; //-- Размер одного фрагмента

      //-- Отправляем первый пакет
      if ( srcFUPart!=RTP_Packet_H264_Unit_FU::FU_START ) { partRTPh264FU.setFUPart(RTP_Packet_H264_Unit_FU::FU_MIDDLE); } //-- Если исходный пакет был начальным, то первый фрагмент то же должен остаться начальным
      partRTPh264FU.setSequence(++_sequence);
      _socket->writeDatagram((const char *)(packetContent), partSize+commonHeaderSize, _host, _port);
      uint64_t partContentOffset =partSize+commonHeaderSize;

      while ( partContentOffset<packetSize) {
        //-- В заголовке прописываем нужные параметры
        partRTPh264FU.setSequence(++_sequence);
        if ( partContentOffset+partSize>=packetSize && srcFUPart==RTP_Packet_H264_Unit_FU::FU_END ) { partRTPh264FU.setFUPart(RTP_Packet_H264_Unit_FU::FU_END);  } //-- Если исходный пакет был конечным, то последний фрагмент то же должен быть конечным
        else { partRTPh264FU.setFUPart(RTP_Packet_H264_Unit_FU::FU_MIDDLE); } //-- Остальные фрагменты промежуточными быть должны
        //-- Копируем заголовок перед данными
        std::memcpy((char *)(packetContent+partContentOffset-commonHeaderSize), packetContent, commonHeaderSize);
        //-- Отправляем
        partSize =qMin(partSize, packetSize-partContentOffset); //-- Учитываем, что последний фрагмент может быть меньше по размеру
        _socket->writeDatagram((const char *)(packetContent+partContentOffset-commonHeaderSize), partSize+commonHeaderSize, _host, _port);
        //-- Идём дальше
        partContentOffset +=partSize;
      }
    }

  } else { //-- Отправляем целиком
    RTP_Packet newRTPPacket(QByteArray(packet->data().data(), packet->size()));
    newRTPPacket.setSequence(++_sequence);
    qint64 dataSendedSize =_socket->writeDatagram(newRTPPacket.data(), _host, _port);
    if ( dataSendedSize<0 ) {}
  }

}

/**
* @brief Основной цикл потока
*/
void Server_Client_Streamer::loop()
{    

}

bool Server_Client_Streamer::onStarted()
{
  if (_socket==nullptr ) {
    _socket = new QUdpSocket(this);
  }

  connect(dynamic_cast<RTSP_Stream*>(_streamer), &RTSP_Stream::newPacketAvaliable, this, &Server_Client_Streamer::onNewPacketAvaliable, Qt::QueuedConnection);

  return WThread::onStarted();
}

Server_Client_Streamer::~Server_Client_Streamer()
{
  qDebug()<<"";
}
