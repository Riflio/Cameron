#include "server_client_streamer.h"

#include <QDebug>

Server_Client_Streamer::Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id, IRTSP_Stream * streamer)
    : WThread(parent), _host(host), _port(port), _id(id), _streamer(streamer)
{
    _buffOffset = -1;
}


int Server_Client_Streamer::id()
{
    return _id;
}

void Server_Client_Streamer::process()
{
    qInfo()<<"";

    _processTimer = new QTimer(this);
    _processTimer->setInterval(1);

    connect(_processTimer, &QTimer::timeout, this, &Server_Client_Streamer::processLoop);

    _socket = new QUdpSocket(this);

    _processTimer->start();

}



/**
* @brief Основной цикл потока
*/
void Server_Client_Streamer::processLoop()
{
    if (status()&WT_PAUSED) return;

    if (!(status()&WT_RUNNING)) {
        _processTimer->stop();
        finish();
        return;
    }

    QByteArray frame;

    if ( !_streamer->getPacketData(_buffOffset, frame) ) return; //-- нет новых фреймов с камеры, курим бамбук

    _socket->writeDatagram(frame, _host, _port);
    //_socket->waitForBytesWritten();

}

Server_Client_Streamer::~Server_Client_Streamer()
{
    qDebug()<<"";
    if (_socket->isOpen()) _socket->close();
}
