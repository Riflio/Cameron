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
    qInfo()<<"Start server"<<_host<<_port;

    _server = new QTcpServer(this);

    connect(_server, &QTcpServer::newConnection, this, &Server::newClient);


    if (! _server->listen(_host, _port) ) {
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

    emit Events->doAction("newClient", QVariantList()<<Events->ARG(client));

    connect(client, &Server_Client::destroyed,  [this, client]() { delClient(client->clientID()); });

    _clients.insert(client->clientID(), client);

}

/**
* @brief Клиент удалился, обрабатываем
*/
void Server::delClient(int clientID)
{
    qDebug()<<clientID;
    _clients.remove(clientID);
}

/**
* @brief Проверяем, имеет ли пользователь доступ к управлению
* @return
*/
bool Server::userHasAccess(Server_Client * client)
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

Server::~Server()
{
    qDebug()<<"";
}
