#include "server.h"
#include <QDebug>

Server::Server(QObject *parent)
    :QObject(parent)
{

}

bool Server::setSettings(QString host, int port)
{
    _host = host;
    _port = port;
    return true;
}

void Server::setCams(ICameras * cameras)
{
    _cameras = cameras;
}

/**
* @brief Запускаем сервер в работу
* @return
*/
bool Server::startServer()
{
    qInfo()<<"Start server at"<<_host<<_port;

    _server = new QTcpServer(this);
    connect(_server, &QTcpServer::newConnection, this, &Server::newClient);
    connect(_server, &QTcpServer::serverError, this, &Server::serverError);


    if ( !_server->listen(_host, _port) ) {
        qWarning()<<_server->errorString();
        return false;
    }

    qInfo()<<"Server started";

    emit Events->doAction("ServerStarted", QVariantList()<<Events->ARG(dynamic_cast<IServer*>(this)));

    return true;
}

/**
* @brief Обрабатываем новое подключение, содаём клиента
*/
void Server::newClient()
{
    qInfo()<<"New client";

    Server_Client * client = new Server_Client(this, _server->nextPendingConnection(), this);

    emit Events->doAction("NewClient", QVariantList()<<Events->ARG(client));

    connect(client, &Server_Client::notAlive, this, &Server::delClient);

    _clients.insert(client->clientID(), client);

}

/**
* @brief Удаляем клиента
*/
void Server::delClient(int clientID)
{
    if ( !_clients.contains(clientID) ) return;

    qInfo()<<"Client disconnected"<<clientID;

    emit Events->doAction("DelClient", QVariantList()<<clientID);

    _clients.value(clientID)->deleteLater();
    _clients.remove(clientID);
}

void Server::serverError()
{
    qWarning()<<"Server error!"<<_server->errorString();
    emit Events->doAction("ServerError", QVariantList());
}

/**
* @brief Проверяем, имеет ли пользователь доступ к управлению
* @return
*/
bool Server::userHasAccess(Server_Client * client) //TODO: Сделать
{

    return true;
}

ICameras * Server::getCams()
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
    _avaliableUsers[name]=pass;
}

QHostAddress Server::host() const
{
    return _host;
}

int Server::port() const
{
    return _port;
}

Server::~Server()
{
}
