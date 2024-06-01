#include "cameronrecorder.h"
#include <QDebug>

bool CameronRecorder::init(ICameron *cameron)
{
  _cameron =cameron;
   qDebug()<<"CameronRipFiles init";

  connect(dynamic_cast<IQSettings*>(_cameron->settings()), &IQSettings::loadSettings, this, &CameronRecorder::onLoadingSettings);
  connect(dynamic_cast<IQCameron*>(_cameron), &IQCameron::started, this, &CameronRecorder::onCameronStarted);

  return true;
}

void CameronRecorder::deInit()
{
  qDebug()<<"deinit";
}

void CameronRecorder::installPlugin()
{
}

void CameronRecorder::unInstallPlugin()
{
}

/**
* @brief При загрузке настроек Камероном прогружаем и проверяем наши настройки
* @param name
* @param xml
*/
void CameronRecorder::onLoadingSettings(QString name, const QXmlStreamReader *xml)
{
  if ( name=="ripFiles" ) {
    QXmlStreamAttributes attributes =xml->attributes();
    TRecorderSettings s;
    s.id =attributes.value("id").toInt();
    qInfo()<<"Loading Recorder settings"<<s.id;
    if ( _settings.contains(s.id) ) {
      qWarning()<<"-- Already exists, skiping";
      return;
    }
    QString camId =attributes.value("camId").toString();
    if ( camId.isEmpty() ) {
      qWarning()<<"-- CamId not found or empty value - skiped";
      return;
    }
    s.camID =attributes.value("camId").toInt();

    s.savePath =attributes.value("savePath").toString();
    QString autoStart =attributes.value("autoStart").toString().toLower();
    if ( autoStart=="on" || autoStart=="yes" || autoStart=="true" || autoStart =="1" ) {
      s.autoStart =true;
    } else
    if ( autoStart=="off" || autoStart=="no" || autoStart=="false" || autoStart =="0" ) {
      s.autoStart =false;
    } else {
      qWarning()<<"-- Invalid foramt autoStart value"<<autoStart<<"- skipped";
      return;
    }
    s.maxFileSize =attributes.value("maxFileSize").toInt()*1024*1024;
    s.maxFilesCount =attributes.value("maxFilesCount").toInt();
    QString newFileAtTime =attributes.value("newFileAtTime").toString();
    if ( !newFileAtTime.isEmpty() ) {
      s.newFileAtTime =QTime::fromString(newFileAtTime);
      if ( !s.newFileAtTime.isValid() ) { qWarning()<<"-- Invalid format newFileAtTime value"<<newFileAtTime<<"- skipped"; return; }
    }

    _settings.insert(s.id, s);
  }
}

/**
* @brief Сервер Камерона запустился
* @param server
*/
void CameronRecorder::onServerStarted(IServer *server)
{
  (void)server;
}

/**
* @brief Камерон запустился полностью, можем запускаться мы
*/
void CameronRecorder::onCameronStarted()
{
  qInfo()<<"onCameronStarted";

  //-- Создаём наших самописцев
  for (QHash<int, TRecorderSettings>::const_iterator i=_settings.constBegin(); i!=_settings.constEnd(); ++i) {
    const TRecorderSettings &settings =i.value();
    qInfo()<<"Init Recorder"<<settings.id;

    ICamera *cam =_cameron->server()->getCams()->getCam(settings.camID);
    if ( cam==nullptr ) { qWarning()<<"Unable get cameraId"<<settings.camID<<"- skipped"; continue; }
    Recorder *recorder =new Recorder(this, cam, settings);
    if ( !recorder->init() ) { continue; }
    _recorders.insert(settings.id, recorder);
    qInfo()<<"Recorder inited";

    if ( settings.autoStart ) { recorder->startRec(); }
  }

}

/**
* @brief Отдаём самописец по идентификатору
* @param id
* @return
*/
IRecorder *CameronRecorder::getRecorder(int id) const
{
  return _recorders.value(id, nullptr);
}
