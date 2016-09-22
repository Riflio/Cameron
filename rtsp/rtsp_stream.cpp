#include "rtsp_stream.h"
#include <QDebug>

namespace NS_RSTP {

RTSP_Stream::RTSP_Stream(QObject * parent, int port):
    RTP(0)
{
    _port = port;
    _started = false;

    _thread = new QThread(0); //-- поток не должен иметь родителя, что бы он не снёс его ненароком, пока выполняется
    connect(_thread, &QThread::started, this, &RTSP_Stream::process); //-- при запуске потока сразу начинаем работу
    connect( this, &RTSP_Stream::finished, _thread, &QThread::quit ); //-- как только вся работа сделана или остановлена, останавливаем поток
    connect(_thread, &QThread::finished, this, &QObject::deleteLater ); //-- как только поток закончен удаляемся
    connect(this, &QObject::destroyed, _thread, &QThread::deleteLater ); //-- Как только удалились, удаляем сам поток

    connect(parent, &QObject::destroyed, this, &RTSP_Stream::stop); //-- как только родитель удаляется, останавливаем поток

    moveToThread(_thread);

}

/**
 * @brief Открываем сокет и читаем всё, что можем
 * без сигналов/слотов т.к. мы в отдельном потоке =)
 */
void RTSP_Stream::process()
{
    _socket = new QUdpSocket(this);


    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(error(QAbstractSocket::SocketError)) ); //-- почему-то новый способ не воспринимает О_о
    connect(_socket, &QUdpSocket::connected, this, &RTSP_Stream::connected);
    connect(_socket, &QUdpSocket::disconnected, this, &RTSP_Stream::disconnected);

    if (!_socket->bind(_port)) { return; }

    while (true) {

        if (!_started) { break; }

        if (_socket->hasPendingDatagrams()) {
            QByteArray data;



            data.resize(_socket->pendingDatagramSize());
            _socket->readDatagram(data.data(), data.size() );

            newPacket(data);
        }

        QApplication::processEvents(); //-- мы не жадные

    }

    _socket->close();
    emit finished();

}


bool RTSP_Stream::start()
{
    if (!_started) {
        _started = true;
        _thread->start();
    }

    return true;
}

void RTSP_Stream::stop()
{
    qInfo()<<"RTSP Stream stop";
    if (!_started) {
        this->deleteLater();
    }

    _started = false;
}

RTSP_Stream::~RTSP_Stream()
{
    qInfo()<<"RTSP Stream deleted";
}



}
