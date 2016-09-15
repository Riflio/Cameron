#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>

#include "settings/settings.h"
#include "Camera/cameras.h"
#include "Server/server.h"


class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = 0);
    ~AppCore();
signals:

public slots:

private:
    Settings * _settings;
    Server * _server;
    Cameras * _cameras;
};

#endif // APPCORE_H
