#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "../Interfaces/iserver.h"
#include "server_client.h"

#include "Camera/cameras.h"

#include "Plugins/PluginEventsWrapper.h"

/**
 * @brief The Server class
 */
class Server : public QObject, public IServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    virtual ~Server();
    bool setSettings(QString host, int port);

    void setCams(ICameras * cameras);
    ICameras * getCams();

signals:

public slots:
    bool startServer();

private slots:
    void newClient();

private:
    QTcpServer * _server;
    QHash<int, Server_Client*> _clients;
    QHostAddress _host;
    int _port;
    ICameras * _cameras;

    friend Server_Client;
};

#endif // SERVER_H
