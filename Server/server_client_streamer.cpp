#include "server_client_streamer.h"

#include <QDebug>

/**
* @brief Server_Client_Streamer::Server_Client_Streamer
* @param parent
* @param host
* @param port
* @param id - айдишник. Соответствует камере.
* @param streamer
*/
Server_Client_Streamer::Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id, IRTSP_Stream * streamer)
    : WThread(parent, "Client stream"), _host(host), _port(port), _id(id), _streamer(streamer)
{

}

int Server_Client_Streamer::id()
{
  return _id;
}

/**
* @brief Есть новый пакет
*/
void Server_Client_Streamer::onNewPacketAvaliable(QSharedPointer<IRTP_Packet> packet)
{
  if ( packet->data().isEmpty() ) { return; }
  _socket->writeDatagram(packet->data(), _host, _port);
}

/**
* @brief Основной цикл потока
*/
void Server_Client_Streamer::loop()
{    

}

bool Server_Client_Streamer::onStarted()
{
  if (_socket==nullptr ) {
    _socket = new QUdpSocket(this);
  }

  connect(dynamic_cast<RTSP_Stream*>(_streamer), &RTSP_Stream::newPacketAvaliable, this, &Server_Client_Streamer::onNewPacketAvaliable, Qt::QueuedConnection);

  return WThread::onStarted();
}

Server_Client_Streamer::~Server_Client_Streamer()
{
  qDebug()<<"";
}
