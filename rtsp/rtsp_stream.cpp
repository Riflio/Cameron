#include "rtsp_stream.h"
#include <QDebug>

namespace NS_RSTP {

RTSP_Stream::RTSP_Stream(QObject * parent) : WThread(parent, "RTSP stream"), RTP(), _socket(nullptr)
{

}

/**
* @brief Сообщаем о возникшей ошибке
*/
void RTSP_Stream::goError()
{
  stop();
  emit errored();
}

/**
* @brief Есть новые данные
*/
void RTSP_Stream::onReadyRead()
{
  while ( _socket->hasPendingDatagrams() ) {
    QByteArray data;
    data.resize(_socket->pendingDatagramSize());
    _socket->readDatagram(data.data(), data.size() );
    newPacket(data);
  }
}

/**
* @brief Основной рабочий цикл потока
*/
void RTSP_Stream::loop()
{

}

bool RTSP_Stream::onStarted()
{
  if ( _socket==nullptr ) {
    _socket = new QUdpSocket(this);
    connect(_socket, &QUdpSocket::connected, this, &RTSP_Stream::connected);
    connect(_socket, &QUdpSocket::disconnected, this, &RTSP_Stream::disconnected);
    connect(_socket, &QUdpSocket::readyRead, this, &RTSP_Stream::onReadyRead);
  }

  if ( !_socket->bind(_port) ) { //-- По каким-то причинам не удалось создать подключение, генерируем ошибку
    qWarning()<<"Unable bind rtsp_stream socket!";
    goError();
    return false;
  }

  return WThread::onStarted();
}

void RTSP_Stream::onFinished()
{
  if ( _socket->isOpen() ) { _socket->close(); }
  WThread::onFinished();
}

RTSP_Stream::~RTSP_Stream()
{

}

void RTSP_Stream::setPort(int port)
{
  _port = port;
}


}
