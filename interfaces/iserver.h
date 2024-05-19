#ifndef ISERVER_H
#define ISERVER_H

#include <QObject>

#include "interfaces/icameras.h"
#include "interfaces/iserverclient.h"

class IServer
{
public:
  virtual ~IServer() {}
  virtual void setCams(ICameras *cameras) =0;
  virtual ICameras *getCams() const=0;

  virtual QHostAddress host() const =0;
  virtual int port() const =0;

signals:
  void started(const IServer *server);
  void errored();
  void clientConnected(const IServerClient *client);
  void clientDisconnected(const IServerClient *client);

};

#endif // ISERVER_H
