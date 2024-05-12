#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include <QObject>
#include <QDir>
#include <QPluginLoader>

#include "plugins/defines.h"

#include "plugins/PluginInterface.h"
#include "plugins/PluginEventsWrapper.h"
#include "plugins/eventer.h"

class PluginsManager : public PluginEventsBase
{
  Q_OBJECT
public:
  explicit PluginsManager(QObject *parent = 0);
  bool loadPlugins(QString path);

signals:
  void doAction(QString, QVariantList);
  void addAction(QString, QObject *, const char *, Qt::ConnectionType connType = Qt::AutoConnection);

public slots:

private slots:

private:
  Eventer * _eventer;
  QString _path;
};

#endif // PLUGINSMANAGER_H
