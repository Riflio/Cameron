#include "settings.h"
#include <QDebug>

Settings::Settings(QObject *parent) : QObject(parent)
{

}


bool Settings::load(Cameras * cameras, Server * server, PluginsManager * pluginsManager)
{
  qInfo()<<"Load settings";

  _settingsFile = new QFile("cameron.xml");
  QFileInfo fileInfo(_settingsFile->fileName());
  qInfo()<<"--path"<<fileInfo.absoluteFilePath();

  if ( !_settingsFile->open(QIODevice::ReadOnly | QIODevice::Text) ) {
    qWarning()<< "Settings file not opened!";
    return false;
  }

  QXmlStreamReader xml(_settingsFile);
  if ( xml.hasError() ) { return false; }
  while ( !xml.atEnd() ) {
    if ( xml.hasError() ) { return false; }
    QXmlStreamReader::TokenType token = xml.readNext();
    if ( token==QXmlStreamReader::StartDocument || token==QXmlStreamReader::EndDocument ) { continue; }
    if ( token==QXmlStreamReader::StartElement ) {
      if ( xml.name()==QStringLiteral("cameras") )  { continue; }
      if ( xml.name()==QStringLiteral("camera") ) {
        QXmlStreamAttributes attributes = xml.attributes();
        if ( !attributes.hasAttribute("id") || !attributes.hasAttribute("url") ) { continue; }
        cameras->newCam(attributes.value("id").toInt())->setSettings(
          attributes.value("url").toString(),
          attributes.value("id").toInt(),
          attributes.value("channel").toInt(),
          attributes.value("streamPort").toInt()
        );
      } else
      if ( xml.name()==QStringLiteral("server") ) {
        QXmlStreamAttributes attributes = xml.attributes();
        server->setSettings(attributes.value("host").toString(), attributes.value("port").toInt());
        if ( attributes.hasAttribute("blockSize") ) { server->setBlockSize(attributes.value("blockSize").toUInt()); }
      } else
      if ( xml.name()==QStringLiteral("plugins") ) {
        QXmlStreamAttributes attributes = xml.attributes();
        pluginsManager->loadPlugins(attributes.value("dir").toString());
      } else
      if ( xml.name()==QStringLiteral("users") ) {
        continue;
      } else
      if ( xml.name()==QStringLiteral("user") ) {
        QXmlStreamAttributes attributes = xml.attributes();
        server->addAvaliableUser(attributes.value("login").toString(), attributes.value("pass").toString());
      }
      emit Events->doAction("LoadingSettings", QVariantList()<< xml.name().toString() << Events->ARG(&xml));
    }
  }
  return true;
}
