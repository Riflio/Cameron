#ifndef ICAMERON_H
#define ICAMERON_H

#include "interfaces/iserver.h"
#include "interfaces/isettings.h"
#include "interfaces/ipluginsmanager.h"

class ICameron
{
public:
  virtual ~ICameron() {}

  virtual QString version() const =0;
  virtual IServer *server() const =0;
  virtual ISettings *settings() const =0;
  virtual IPluginsManager *pluginsManager() const =0;

signals:
  virtual void started() =0;

};

#endif // ICAMERON_H
