#include "appcore.h"
#include <QDebug>

AppCore::AppCore(QObject *parent) : QObject(parent)
{
    _settings = new Settings(this);

    _cameras = new Cameras(this);
    _server = new Server(this, _cameras);


    if (!_settings->load(_cameras, _server)) {
        qWarning()<< "Error load setitngs file.";
    }

    if (!_server->startServer()) {
        qWarning()<<"Unable start server";
    }



}

AppCore::~AppCore()
{
    qDebug()<<"DELETE APPCORE";

}
