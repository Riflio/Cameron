#include "pluginsmanager.h"
#include <QDebug>

PluginEvents *PluginEvents::_sInst = 0;

PluginsManager::PluginsManager(QObject *parent) : PluginEventsBase(parent)
{
    _eventer = new Eventer(this);
    connect(this, SIGNAL(doAction(QString,QVariantList)), _eventer, SLOT(doAction(QString,QVariantList)) );
    connect(this, SIGNAL(addAction(QString,QObject*,const char*)), _eventer, SLOT(addAction(QString,QObject*,const char*)) );

    new PluginEvents(this);
}

void PluginsManager::setPath(QString path)
{
    qInfo()<<"Set plugins path"<<path;
    _path = path;
}

bool PluginsManager::loadPlugins()
{
    qInfo()<<"Load plugins";

    QDir dir(QDir::currentPath());

    if (!dir.cd(_path))
    {
        qWarning()<<"Plugins directory does not exist"<< _path;
        return false;
    }

    foreach (QString strFileName, dir.entryList(QDir::Files))
    {
        QPluginLoader loader(dir.absoluteFilePath(strFileName));

        PluginInterface * pI = qobject_cast<PluginInterface*>(loader.instance());
        if (!pI)
        {
            qWarning()<<"Module NOT loaded: "<<loader.errorString();
            continue;
        } else {
            qInfo()<<"Module loaded: "<<strFileName;
        }

        QString plName = loader.metaData().value("MetaData").toObject().value("Name").toString();
        QString plDescr = loader.metaData().value("MetaData").toObject().value("Descr").toString();
        QString plVersion = loader.metaData().value("MetaData").toObject().value("Version").toString();


        connect(loader.instance(), SIGNAL(doAction(QString, QVariantList)), Events, SIGNAL(doAction(QString, QVariantList)));
        connect(loader.instance(), SIGNAL(addAction(QString, QObject*, const char*)), Events, SIGNAL(addAction(QString, QObject*, const char*)));
        pI->Init();
    }

    emit Events->doAction("Inited", QVariantList());


    return true;

}
