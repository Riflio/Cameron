#include "appcore.h"
#include <QDebug>


AppCore::AppCore(QObject *parent) : QObject(parent)
{

}


void AppCore::appStart()
{
    _settings = new Settings(this);

    _server = new Server(this);
    _cameras = new Cameras(this);
    _pluginsManager = new PluginsManager(this);

    _server->setCams(_cameras);


    if (!_settings->load(_cameras, _server, _pluginsManager)) {
        qWarning()<< "Error load setitngs file.";
    }

    if (!_pluginsManager->loadPlugins()) {
        qInfo()<<"Plugins not loaded";
    }

    if (!_server->startServer()) {
        qWarning()<<"Unable start server";
    }

}



AppCore::~AppCore()
{

    qInfo()<<"AppCore deleted";
}
