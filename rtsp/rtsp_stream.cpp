#include "rtsp_stream.h"
#include "rtp/rtp_packet.h"
#include <QDebug>

namespace NS_RSTP {

RTSP_Stream::RTSP_Stream(QObject * parent): QObject(parent)
{

}

/**
* @brief Сообщаем о возникшей ошибке
*/
void RTSP_Stream::goError()
{
  emit errored();
}

/**
* @brief Есть новые данные
*/
void RTSP_Stream::onReadyRead()
{
  while ( _socket->hasPendingDatagrams() ) {
    RTP_Packet *packet =new RTP_Packet(_socket->pendingDatagramSize());
    _socket->readDatagram(packet->data().data(), packet->data().size());
    emit newPacketAvaliable(QSharedPointer<IRTP_Packet>(packet));
  }
}

/**
* @brief Запускают поток
*/
void RTSP_Stream::start()
{
  if ( _socket==nullptr ) {
    _socket =new QUdpSocket(this);
    connect(_socket, &QUdpSocket::connected, this, &RTSP_Stream::connected);
    connect(_socket, &QUdpSocket::disconnected, this, &RTSP_Stream::disconnected);
    connect(_socket, &QUdpSocket::readyRead, this, &RTSP_Stream::onReadyRead);
  }

  if ( !_socket->bind(_port) ) { //-- По каким-то причинам не удалось создать подключение, генерируем ошибку
    qWarning()<<"Unable bind rtsp_stream socket!";
    goError();
  }

}

RTSP_Stream::~RTSP_Stream()
{
  if ( _socket!=nullptr && _socket->isOpen() ) { _socket->close(); }
}

void RTSP_Stream::setPort(int port)
{
  _port =port;
}

}
