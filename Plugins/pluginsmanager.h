#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include "Plugins/PluginInterface.h"
#include "Plugins/PluginEventsWrapper.h"
#include "Plugins/eventer.h"

class PluginsManager : public PluginEventsBase
{
    Q_OBJECT
public:
    explicit PluginsManager(QObject *parent = 0);
    bool loadPlugins();

signals:

public slots:
    void setPath(QString path);

private:
    QString _path;
    Eventer * _eventer;
};

#endif // PLUGINSMANAGER_H
