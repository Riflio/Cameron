#include "server_client_streamer.h"

#include <QDebug>

Server_Client_Streamer::Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id, IRTSP_Stream * streamer)
    : WThread(parent, "Client stream"), _host(host), _port(port), _id(id), _streamer(streamer)
{
    _buffOffset = -1;
}


int Server_Client_Streamer::id()
{
    return _id;
}


/**
* @brief Основной цикл потока
*/
void Server_Client_Streamer::loop()
{    

    const QByteArray * frame = _streamer->getPacketData(_buffOffset);
    if ( frame==nullptr ) { return; } //-- Нет новых фреймов с камеры, курим бамбук

    _socket->writeDatagram(*frame, _host, _port);
    _socket->waitForBytesWritten();
}

bool Server_Client_Streamer::onStarted()
{
    _socket = new QUdpSocket(this);
    return WThread::onStarted();
}

Server_Client_Streamer::~Server_Client_Streamer()
{
    qDebug()<<"";
    if ( _socket->isOpen() ) _socket->close();
}
