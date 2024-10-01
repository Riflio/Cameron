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
          packet =packetH264FU;
        } else {
          RTPPacketH264 *packetH264 =static_cast<RTPPacketH264 *>(packetH264FU);
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

}
