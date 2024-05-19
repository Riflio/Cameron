#include "server.h"
#include <QDebug>

Server::Server(QObject *parent): QObject(parent)
{
  _server =new QTcpServer(this);
  connect(_server, &QTcpServer::newConnection, this, &Server::newClient);
  connect(_server, &QTcpServer::acceptError, this, &Server::serverError);
}

Server::~Server()
{

}

bool Server::setSettings(QString host, int port)
{
  _host =QHostAddress(host);
  _port =port;
  return true;
}

void Server::setCams(ICameras * cameras)
{
  _cameras =cameras;
}

/**
* @brief Запускаем сервер в работу
* @return
*/
bool Server::startServer()
{
  qInfo()<<"Start server at"<<_host<<_port;
  if ( !_server->listen(_host, _port) ) {
    qWarning()<<_server->errorString();
    return false;
  }
  qInfo()<<"Server started";
  emit started(this);
  return true;
}

/**
* @brief Обрабатываем новое подключение, содаём клиента
*/
void Server::newClient()
{
  qInfo()<<"New client";
  ServerClient *client =new ServerClient(this, _server->nextPendingConnection(), this, _lastClientID++);
  connect(client, &ServerClient::notAlive, this, &Server::delClient);
  _clients.insert(client->clientID(), client);
  emit clientConnected(client);
}

/**
* @brief Удаляем клиента
*/
void Server::delClient(int clientID)
{
  if ( !_clients.contains(clientID) ) { return; }
  qInfo()<<"Client disconnected"<<clientID;
  ServerClient *client =_clients.value(clientID);
  emit clientDisconnected(client);
  client->deleteLater();
  _clients.remove(clientID);
}

/**
* @brief Server::serverError
*/
void Server::serverError()
{
  qWarning()<<"Server error!"<<_server->errorString();
  emit errored();
}

/**
* @brief Проверяем, имеет ли пользователь доступ к управлению
* @return
* @todo Сделать
*/
bool Server::userHasAccess(ServerClient *client)
{
  Q_UNUSED(client);
  return true;
}

/**
* @brief Server::getCams
* @return
*/
ICameras *Server::getCams() const
{
  return _cameras;
}

/**
* @brief Добавляем возможного пользователя сервера
* @param name
* @param pass
*/
void Server::addAvaliableUser(QString name, QString pass)
{
  _avaliableUsers[name] =pass;
}

/**
* @brief Server::host
* @return
*/
QHostAddress Server::host() const
{
  return _host;
}

/**
* @brief Server::port
* @return
*/
int Server::port() const
{
  return _port;
}

/**
* @brief Отдаём дефолтный максимальный размер пакета, что бы влез в MTU, см. Server_Client_Streamer::onNewPacketAvaliable
* @return
*/
uint32_t Server::blockSize() const
{
  return _blockSize;
}

/**
* @brief Задаём дефолтный максимальный размер пакета, что бы влез в MTU, см. Server_Client_Streamer::onNewPacketAvaliable
* @param blockSize
*/
void Server::setBlockSize(uint32_t blockSize)
{
  _blockSize =qMax(200U, blockSize);
}
