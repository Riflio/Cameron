#include "appcore.h"
#include <QDateTime>
#include <QDebug>

AppCore::AppCore(QObject *parent): IQCameron(parent)
{

}

AppCore::~AppCore()
{

}

/**
* @brief Запускаем Cameron
*/
bool AppCore::appStart()
{
  qSetMessagePattern(">>>>%{time yyyyMMdd h:mm:ss.zzz} %{type} %{file} %{function} %{message}"); //-- Шаблон сообщений лога
  //qInstallMessageHandler(myMessageHandler); //-- Перенаправим вывод лога

  qInfo()<<"Start Cameron, version"<<VERSION;

  _settings =new Settings(this);
  _server =new Server(this);
  _cameras =new Cameras(this);
  _pluginsManager =new PluginsManager(this, this);
  _server->setCams(_cameras);

  if ( !_settings->load(_cameras, _server, _pluginsManager) ) { qWarning()<< "Error load setitngs file."; return false; }

  if ( !_server->startServer() ) { qWarning()<<"Unable start server"; return false; }

  qInfo()<<QString("For connect use 'rtsp://admin:pass@%1:%2/track/1'").arg(_server->host().toString()).arg(_server->port());

  emit started();
  return true;
}


void AppCore::myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
  //-- что бы в релизе выводился context нужно в *.pro добавить DEFINES += QT_MESSAGELOGCONTEXT
  QString sType="";
  switch (type) {
    case QtInfoMsg: { sType ="Info"; break; }
    case QtDebugMsg: { sType ="Debug"; break; }
    case QtWarningMsg: { sType ="Warning"; break; }
    case QtCriticalMsg: { sType ="CRITICAL"; break; }
    case QtFatalMsg: { sType ="FATAL"; break; }
  }
  QDateTime dateTime = QDateTime::currentDateTime();
  // Log::instance().msg(dateTime.toString("yyyyMMdd h:mm:ss.zzz"), sType, context.category, QString("%1:%2").arg(context.function).arg(context.line), context.file, msg);
}

/**
* @brief Отдаём нашу версию
* @return
*/
QString AppCore::version() const
{
  return VERSION;
}

/**
* @brief AppCore::server
* @return
*/
IServer *AppCore::server() const
{
  return _server;
}

/**
* @brief AppCore::settings
* @return
*/
ISettings *AppCore::settings() const
{
  return _settings;
}

/**
* @brief AppCore::pluginsManages
* @return
*/
IPluginsManager *AppCore::pluginsManager() const
{
  return _pluginsManager;
}



