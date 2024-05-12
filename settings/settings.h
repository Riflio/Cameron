#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

#include "server/server.h"
#include "camera/cameras.h"
#include "plugins/pluginsmanager.h"
#include "plugins/PluginEventsWrapper.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    bool load(Cameras * cameras, Server * server, PluginsManager * pluginsManager);

signals:

public slots:

private:
    QFile* _settingsFile;

};

#endif // SETTINGS_H
