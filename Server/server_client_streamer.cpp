#include "server_client_streamer.h"

#include <QDebug>

Server_Client_Streamer::Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id, Cameras_Camera * cam)
    : QObject(0),  _host(host), _port(port), _id(id), _thread(0), _cam(cam)
{

    _thread = new QThread(0); //-- поток не должен иметь родителя, что бы он не снёс его ненароком, пока выполняется
    connect(_thread, &QThread::started, this, &Server_Client_Streamer::process); //-- при запуске потока сразу начинаем работу    
    connect( this, &Server_Client_Streamer::finished, _thread, &QThread::quit ); //-- как только вся работа сделана или остановлена, останавливаем поток
    connect(_thread, &QThread::finished, this, &QObject::deleteLater ); //-- как только поток закончен удаляемся
    connect(this, &QObject::destroyed, _thread, &QThread::deleteLater ); //-- Как только удалились, удаляем сам поток

    connect(parent, &QObject::destroyed, this, &Server_Client_Streamer::stop); //-- как только родитель удаляется, останавливаем поток

    _buffOffset = -1;
    _started = false;

    moveToThread(_thread);
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

    RTSP_Stream * streamer =  _cam->getStreamer();

    while (true) {
        if (!_started) { break; }

        QByteArray frame;

        if ( !streamer->getPacket(_buffOffset, frame) ) continue; //-- нет новых фреймов с камеры, курим бамбук

        _socket->writeDatagram(frame, _host, _port);
        _socket->waitForBytesWritten();

         QApplication::processEvents(); //-- мы не жадничаем
    }

    //--
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

    //-- ждём, пока запустится
    while ( !(_cam->status() & Cameras_Camera::S_STARTED) ) { QApplication::processEvents(); }

    //-- подготавливаем камеру к вещанию
    if (!_cam->setup()) return false;

    //-- ждём, пока поток камеры настраивается
    while ( !(_cam->status() & Cameras_Camera::S_SETUPED) ) { QApplication::processEvents(); }

    //-- запускаем вещание
    if (!_cam->play()) return false;

    //-- и ждём, пока начнётся
    while ( !(_cam->status() & Cameras_Camera::S_PLAYED) ) { QApplication::processEvents(); }

    _started = true;

    _thread->start();

    return true;
}


/**
 * @brief Останавливаем камеру и останавливаем поток
 */
void Server_Client_Streamer::stop()
{    
    qInfo()<<"Streamer STOP";
    if (!_started) {
        this->deleteLater();
    }

    _cam->stop();
    _started = false;
}

Server_Client_Streamer::~Server_Client_Streamer()
{
    qInfo()<<"Server_Client_Streamer deleted";
}
