#include "pluginsmanager.h"
#include <QDebug>

PluginEvents *PluginEvents::_sInst = 0;

PluginsManager::PluginsManager(QObject *parent) : PluginEventsBase(parent)
{
    _eventer = new Eventer(this);
    connect(this, SIGNAL(doAction(QString,QVariantList)), _eventer, SLOT(doAction(QString,QVariantList)), Qt::DirectConnection);
    connect(this, SIGNAL(addAction(QString,QObject*,const char*,Qt::ConnectionType)), _eventer, SLOT(addAction(QString,QObject*,const char*,Qt::ConnectionType)), Qt::DirectConnection);
    new PluginEvents(this);
    emit Events->addAction("getWThreadClass", this, SLOT(getWThreadClass(WThread*)), Qt::DirectConnection);

}

/**
* @brief Задаём реализацию функциям потока
* @param p
*/
void PluginsManager::getWThreadClass(WThread * wthread)
{
    W_FUNCT_REL(WThread, init, wthread);
    W_FUNCT_REL(WThread, start, wthread);
    W_FUNCT_REL(WThread, stop, wthread);
    W_FUNCT_REL(WThread, pause, wthread);
    W_FUNCT_REL(WThread, onFinished, wthread);
    W_FUNCT_REL(WThread, onStarted, wthread);
    W_FUNCT_REL(WThread, onAfterStarted, wthread);
    W_FUNCT_REL(WThread, process, wthread);


    W_FUNCT_REL(WThread, id, wthread);
    W_FUNCT_REL(WThread, setID, wthread);
    W_FUNCT_REL(WThread, status, wthread);
    W_FUNCT_REL(WThread, msg, wthread);
    W_FUNCT_REL(WThread, name, wthread);
    W_FUNCT_REL(WThread, progress, wthread);
    W_FUNCT_REL(WThread, setProgress, wthread);
    W_FUNCT_REL(WThread, setMsg, wthread);
}


bool PluginsManager::loadPlugins(QString path)
{
    qInfo()<<"Load plugins";

    QDir dir(QDir::currentPath());

    if (!dir.cd(path))
    {
        qWarning()<<"Plugins directory does not exist"<< path;
        return false;
    }

    foreach (QString strFileName, dir.entryList(QStringList()<<"*.so", QDir::Files))
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
