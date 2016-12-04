#include "server_client_streamer.h"

#include <QDebug>

Server_Client_Streamer::Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id, Cameras_Camera * cam)
    : WThread(parent), _host(host), _port(port), _id(id), _cam(cam)
{
    _buffOffset = -1;
}


int Server_Client_Streamer::id()
{
    return _id;
}

void Server_Client_Streamer::process()
{
    qInfo()<<"Client streamer process"<<_cam->getStreamer();

    _socket = new QUdpSocket(this);
    //TODO: добавить обработку ошибок потока

    IRTSP_Stream * streamer =  _cam->getStreamer();

    while (true) {
        if (!_started) { break; }

        QByteArray frame;

        if ( !streamer->getPacketData(_buffOffset, frame) ) continue; //-- нет новых фреймов с камеры, курим бамбук

        _socket->writeDatagram(frame, _host, _port);
        _socket->waitForBytesWritten();

         QApplication::processEvents(); //-- мы не жадничаем
    }

    _socket->close();
    emit finished( id() );

}

Cameras_Camera * Server_Client_Streamer::cam()
{
    return _cam;
}

/**
 * @brief Ждём запуска камеры и запускаем поток
 */
bool Server_Client_Streamer::start()
{
    qInfo()<<"Client streamer start";

    if ( _started ) return true;

    //-- запускаем камеру, если ещё не запущена
    if (!_cam->start()) return false;

    //-- ждём, пока приконнектится
    while ( !(_cam->status() & Cameras_Camera::S_CONNECTED) ) { QApplication::processEvents(); }

    //-- подготавливаем камеру к вещанию
    if (!_cam->setup()) return false;

    //-- ждём, пока поток камеры настраивается
    while ( !(_cam->status() & Cameras_Camera::S_SETUPED) ) { QApplication::processEvents(); }

    //-- запускаем вещание
    if (!_cam->play()) return false;

    //-- и ждём, пока начнётся
    while ( !(_cam->status() & Cameras_Camera::S_PLAYED) ) { QApplication::processEvents(); }

    WThread::start();

    return true;
}


/**
 * @brief Останавливаем камеру и останавливаем поток
 */
bool Server_Client_Streamer::stop()
{    
    qInfo()<<"Streamer STOP";

    _cam->stop();

    return WThread::stop();
}

Server_Client_Streamer::~Server_Client_Streamer()
{
    qInfo()<<"Server_Client_Streamer deleted";
}
