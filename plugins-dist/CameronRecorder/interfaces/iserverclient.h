#ifndef ISERVERCLIENT_H
#define ISERVERCLIENT_H

#include <QObject>
#include "iserverclientinfo.h"

class IServerClient: public virtual IServerClientInfo
{
public:
  virtual ~IServerClient() {}

};

#endif // ISERVERCLIENT_H
