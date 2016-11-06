#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

#include "Server/server.h"
#include "Camera/cameras.h"
#include "Plugins/pluginsmanager.h"


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
