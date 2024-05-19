#include "pluginsmanager.h"
#include "interfaces/iplugininterface.h"

#include <QDebug>

PluginsManager::PluginsManager(ICameron *cameron, QObject *parent): QObject(parent), _cameron(cameron)
{

}

/**
* @brief Загружаем плагины
* @param path
* @return
*/
bool PluginsManager::loadPlugins(QString path)
{
  qInfo()<<"Load plugins";
  QDir dir(QDir::currentPath());

  if ( !dir.cd(path) ) {
    qWarning()<<"Plugins directory does not exist"<< path;
    return false;
  }

  foreach (QString strFileName, dir.entryList(QStringList()<<"*.so", QDir::Files)) {
    QPluginLoader loader(dir.absoluteFilePath(strFileName));
    IPluginInterface *pI=qobject_cast<IPluginInterface*>(loader.instance());
    if ( pI==nullptr ) {
      qWarning()<<"Module NOT loaded: "<<loader.errorString();
      continue;
    } else {
      qInfo()<<"Module loaded: "<<strFileName;
    }

    QString plName =loader.metaData().value("MetaData").toObject().value("Name").toString();
    QString plDescr =loader.metaData().value("MetaData").toObject().value("Descr").toString();
    QString plVersion =loader.metaData().value("MetaData").toObject().value("Version").toString();

    if ( _plugins.contains(plName) ) { loader.unload(); continue; } //-- Уже есть одна копия, выгружаем новую

    if ( !pI->init(_cameron) ) { qWarning()<<"Unable init plugin"<<plName; continue; }

    _plugins[plName] =pI;
  }

  return true;
}

/**
* @brief Выгружаем плагины
*/
void PluginsManager::unloadPlugins() //FIXME: Сделать
{

}

/**
* @brief Отдаём список загруженных плагинов
* @return
*/
IPluginsManager::TPlugins PluginsManager::plugins() const
{
  return _plugins;
}
