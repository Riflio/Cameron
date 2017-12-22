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

    void addAvaliableUser(QString name, QString pass);

signals:

public slots:
    bool startServer();

private slots:
    void newClient();
    void delClient(int clientID);

private:
    QTcpServer * _server;
    QHash<int, Server_Client*> _clients;
    QHostAddress _host;
    int _port;
    ICameras * _cameras;

    bool userHasAccess(Server_Client *);

    QHash<QString, QString> _avaliableUsers;

    friend Server_Client;
};

#endif // SERVER_H
