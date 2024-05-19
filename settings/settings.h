#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>

#include "interfaces/iqsettings.h"
#include "server/server.h"
#include "camera/cameras.h"
#include "plugins/pluginsmanager.h"

class Settings: public IQSettings
{
  Q_OBJECT
public:
  explicit Settings(QObject *parent =nullptr);
  bool load(Cameras *cameras, Server *server, PluginsManager *pluginsManager);

signals:

public slots:

private:
  QFile* _settingsFile;

};

#endif // SETTINGS_H
