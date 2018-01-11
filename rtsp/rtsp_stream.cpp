#include "rtsp_stream.h"
#include <QDebug>

namespace NS_RSTP {

RTSP_Stream::RTSP_Stream(QObject * parent, int port):
  WThread(parent, "RTSP stream"), RTP()
{
    _port = port;    
}

/**
 * @brief Открываем сокет и читаем всё, что можем
 * без сигналов/слотов т.к. мы в отдельном потоке =)
 */
void RTSP_Stream::process()
{
    _socket = new QUdpSocket(this);

    connect(_socket, &QUdpSocket::connected, this, &RTSP_Stream::connected);
    connect(_socket, &QUdpSocket::disconnected, this, &RTSP_Stream::disconnected);

    if (!_socket->bind(_port)) { //-- По каким-то причинам не удалось создать подключение, генерируем ошибку
         goError();
         return;
    }

    _processTimer = new QTimer(this);
    _processTimer->setInterval(3);
    connect(_processTimer, &QTimer::timeout, this, &RTSP_Stream::processLoop);
    _processTimer->start();



}


/**
* @brief Сообщаем о возникшей ошибке
*/
void RTSP_Stream::goError()
{
    emit errored();
    finished();
}

/**
* @brief Основной рабочий цикл потока
*/
void RTSP_Stream::processLoop()
{
    if (!(_status & WT_RUNNING)) {
        _processTimer->stop();
        finish();
        return;
    }

    if (_status & WT_PAUSED) return;

    if (_socket->hasPendingDatagrams()) {
        QByteArray data;

        data.resize(_socket->pendingDatagramSize());
        _socket->readDatagram(data.data(), data.size() );

        newPacket(data);
    }
}

RTSP_Stream::~RTSP_Stream()
{
    qDebug()<<"";
    if (_socket->isOpen()) _socket->close();
}



}
