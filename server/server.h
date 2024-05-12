#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "interfaces/iserver.h"
#include "server_client.h"

#include "camera/cameras.h"
#include "plugins/PluginEventsWrapper.h"

/**
* @brief The Server class
*/
class Server : public QObject, public IServer
{
  Q_OBJECT
public:
  explicit Server(QObject *parent =nullptr);
  virtual ~Server();
  bool setSettings(QString host, int port);

  void setCams(ICameras *cameras);
  ICameras *getCams();

  void addAvaliableUser(QString name, QString pass);

  QHostAddress host() const;
  int port() const;

  uint32_t blockSize() const;
  void setBlockSize(uint32_t blockSize);

signals:

public slots:
  bool startServer();

private slots:
  void newClient();
  void delClient(int clientID =-1);
  void serverError();

private:
  QTcpServer *_server =nullptr;
  QHash<uint32_t, Server_Client*> _clients;
  uint32_t _lastClientID =0; //-- Инкрементный ИД клиента для списка _clients, что бы потом нам не путать от кого что было
  QHostAddress _host;
  int _port;
  ICameras *_cameras =nullptr;
  uint32_t _blockSize =1400; //-- Дефолтный максимальный размер пакета, что бы влез в MTU, см. Server_Client_Streamer::onNewPacketAvaliable
  QHash<QString, QString> _avaliableUsers;

  bool userHasAccess(Server_Client *);

  friend Server_Client;
};

#endif // SERVER_H
