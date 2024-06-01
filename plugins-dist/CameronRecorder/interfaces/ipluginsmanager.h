#ifndef IPLUGINSMANAGER_H
#define IPLUGINSMANAGER_H

#include <QObject>

class IPluginInterface;
class IPluginsManager
{
public:
  virtual ~IPluginsManager() {}

  typedef QMap<QString, IPluginInterface*> TPlugins;

  virtual TPlugins plugins() const =0;

};

#endif // IPLUGINSMANAGER_H
