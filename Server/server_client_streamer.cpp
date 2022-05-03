#include "server_client_streamer.h"

#include <QDebug>

Server_Client_Streamer::Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id, IRTSP_Stream * streamer)
    : WThread(parent, "Client stream"), _host(host), _port(port), _id(id), _streamer(streamer)
{
  _streamerFrames = new CircleBufferReader<IRTP_Packet>(_streamer->rtpPacketsBuffer());
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
  const IRTP_Packet * packet = _streamerFrames->get();
  if ( packet==nullptr ) { return; }
  QByteArray data = packet->data();
  if ( data.isEmpty() ) { return; }
  _socket->writeDatagram(data, _host, _port);
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
}
