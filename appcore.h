#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include "settings/settings.h"
#include "Camera/cameras.h"
#include "Server/server.h"
#include "Plugins/pluginsmanager.h"

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);
    ~AppCore();

    static void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString &msg);

signals:

public slots:
    void appStart();

private:
    Settings * _settings;
    Server * _server;
    Cameras * _cameras;
    PluginsManager * _pluginsManager;

};

#endif // APPCORE_H
