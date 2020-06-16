#include "rtsp_stream.h"
#include <QDebug>

namespace NS_RSTP {

RTSP_Stream::RTSP_Stream(QObject * parent, int port):
  WThread(parent, "RTSP stream"), RTP()
{
    _port = port;    
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
* @brief Основной рабочий цикл потока
*/
void RTSP_Stream::loop()
{

    if ( _socket->hasPendingDatagrams() ) {
        QByteArray data;
        data.resize(_socket->pendingDatagramSize());
        _socket->readDatagram(data.data(), data.size() );
        newPacket(data);
    }
}

bool RTSP_Stream::onStarted()
{
    _socket = new QUdpSocket(this);

    connect(_socket, &QUdpSocket::connected, this, &RTSP_Stream::connected);
    connect(_socket, &QUdpSocket::disconnected, this, &RTSP_Stream::disconnected);

    if ( !_socket->bind(_port) ) { //-- По каким-то причинам не удалось создать подключение, генерируем ошибку
         goError();
         return false;
    }

    return WThread::onStarted();
}

RTSP_Stream::~RTSP_Stream()
{
    if ( _socket->isOpen() ) { _socket->close(); }
}


}
