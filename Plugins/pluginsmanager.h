#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include <QObject>
#include <QDir>
#include <QPluginLoader>

#include "Plugins/defines.h"

#include "Plugins/PluginInterface.h"
#include "Plugins/PluginEventsWrapper.h"
#include "Plugins/eventer.h"

#include "Plugins/wthread.h"


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
    void getWThreadClass(WThread * wthread);

private:
    Eventer * _eventer;
    QString _path;
};

#endif // PLUGINSMANAGER_H
