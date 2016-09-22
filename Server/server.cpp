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
}

void Server::setCams(Cameras * cameras)
{
    _cameras = cameras;
}

/**
 * @brief Запускаем сервер в работу
 * @return
 */
bool Server::startServer()
{
    qInfo()<<"Start server";

    _server = new QTcpServer(this);

    connect(_server, &QTcpServer::newConnection, this, &Server::newClient  );


    if (! _server->listen(_host, _port) ) {
        qWarning()<<"Unable start server"<<_server->errorString();
        return false;
    }

    qInfo()<<"Server started";

    return true;
}

/**
 * @brief Обрабатываем новое подключение
 */
void Server::newClient()
{
    qInfo()<<"New client";

    QTcpSocket* clientSocket = _server->nextPendingConnection();

    Server_Client * client = new Server_Client(this, clientSocket, this);

    connect(clientSocket, &QTcpSocket::readyRead, client, &Server_Client::request);

    _clients.insert(client->clientID(), client);

}

Server::~Server()
{
    qInfo()<<"Server deleted";
}
