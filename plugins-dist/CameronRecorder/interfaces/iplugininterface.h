#ifndef IPLUGININTERFACE_H
#define IPLUGININTERFACE_H

#include <QObject>
#include "interfaces/icameron.h"

class IPluginInterface
{
public:
  virtual ~IPluginInterface() {}

  virtual bool init(ICameron *cameron) =0;
  virtual void deInit() =0;

  virtual void installPlugin() =0;
  virtual void unInstallPlugin() =0;

public slots:

signals:

};
Q_DECLARE_INTERFACE(IPluginInterface, "com.Cameron.Application.PluginInterface/1.0")

#endif // IPLUGININTERFACE_H
