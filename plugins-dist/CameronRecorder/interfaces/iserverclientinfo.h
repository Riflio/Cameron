#ifndef ISERVERCLIENTINFO_H
#define ISERVERCLIENTINFO_H

#include <QObject>

class IServerClientInfo {
public:
  virtual ~IServerClientInfo() {}
  virtual QString getName() const =0;
  virtual bool isActual() const =0;;
};

#endif // ISERVERCLIENTINFO_H
