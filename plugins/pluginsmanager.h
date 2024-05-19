#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include <QObject>
#include <QDir>
#include <QPluginLoader>

#include "interfaces/ipluginsmanager.h"
#include "interfaces/icameron.h"

class PluginsManager: public QObject, public IPluginsManager
{
  Q_OBJECT
public:
  explicit PluginsManager(ICameron *cameron, QObject *parent =nullptr);
  bool loadPlugins(QString path);
  void unloadPlugins();

  TPlugins plugins() const override;

signals:

public slots:

private slots:

private:
  ICameron *_cameron =nullptr;
  TPlugins _plugins;

};

#endif // PLUGINSMANAGER_H
