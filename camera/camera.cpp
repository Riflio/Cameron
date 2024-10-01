#include "camera.h"
#include <QCoreApplication>

Camera::Camera(QObject *parent): IQCamera(parent)
{
  qInfo()<<"Camera new";
  _rtsp =new RTSP(this);
  connect(_rtsp, &RTSP::connected, this, &Camera::onCameraConnected);
  connect(_rtsp, &RTSP::disconnected, this, &Camera::onCameraDisconnected);
  connect(_rtsp, &RTSP::errored, this, &Camera::onCameraErrored);
}

int Camera::id()
{
  return _id;
}

int Camera::status()
{
  return _status;
}

bool Camera::setSettings(QString url, int id, int channel, int streamPort)
{
  qInfo()<<"Camera set settings "<<url;
  _id =id;
  _url =url;
  _channel =channel;
  _streamPort =streamPort;
  return true;
}

/**
* @brief Подключаемся к камере
* @return
*/
bool Camera::start()
{
  if ( _url.isEmpty() ) { return false; }

  if ( _status&S_STARTED ) { return true; }

  qInfo()<<"Camera start ";

  _rtsp->cameraConnect(_url);

  _status &=~S_ERROR; //-- Уберём ошибку в случае попытки переподключения
  _status |=S_STARTED;

  return true;
}

/**
* @brief Удачно подключились к камере
*/
void Camera::onCameraConnected()
{
  qInfo()<<"Camera connected ";
  _status |=S_CONNECTED;

  emit connected();
}

/**
* @brief Отключились от камеры (сами или камера закрыла соединение)
*/
void Camera::onCameraDisconnected()
{
  qInfo()<<"Camera disconnected";
  _status =S_NONE;

  emit disconnected();
}

/**
* @brief Возникли ошибки при общении с камерой
*/
void Camera::onCameraErrored()
{    
  qWarning()<<"Camera "<<id()<<"error!";
  _status =S_NONE;
  _status |=S_ERROR;

  _rtsp->cameraDisconnect();

  emit errored();
}

/**
* @brief Подготавливаем камеру к вещанию
* @return
*/
bool Camera::setup()
{
  if ( _status&S_SETUPED ) { return true; }
  if ( _status&S_ERROR ) { return false; }

  qInfo()<<"";

  RTSP_Channel *channel =_rtsp->getChannel(_channel);
  if ( channel==nullptr ) { qWarning()<<"Channel"<<channel->id()<<"not found!"; return false; }

  connect(channel, &RTSP_Channel::setuped, this, &Camera::onSetuped, Qt::UniqueConnection);
  connect(channel, &RTSP_Channel::played, this, &Camera::onPlayed, Qt::UniqueConnection);
  connect(channel, &RTSP_Channel::toTeardown, this, &Camera::onTeardowned, Qt::UniqueConnection);

  channel->setup(_streamPort);

  return true;
}

/**
* @brief Удачно выставили настройки
*/
void Camera::onSetuped()
{
  qInfo()<<"Camera setuped";
  _status |=S_SETUPED;

  emit setuped();
}

/**
* @brief Запускаем передачу видео
* @return
*/
bool Camera::play()
{
  qInfo()<<"Camera play";
  if ( _status&S_ERROR ) { return false; }
  _clientsCount++;
  if ( _status&S_PLAYED ) { return true; }
  _rtsp->getChannel(_channel)->play();
  return true;
}

/**
* @brief Прекращаем вещание
* @param force - завершить принудительно (да же если кто-то использует)
* @return
*/
bool Camera::stop(bool force)
{
  qInfo()<<"Camera stop, clients count:"<<_clientsCount;
  _clientsCount--;

  if ( _status&S_PLAYED ) { //-- Если запущена
    //-- И принудительная остановка или нет больше клиентов подключившихся
    if ( force || _clientsCount<=0 ) {
      _rtsp->getChannel(_channel)->teardown();
      _status =S_NONE;
      _clientsCount =0;
    }
  } else {
    _clientsCount =0;
  }

  return true;
}

/**
* @brief Делаем всё, что бы запустить вещание камеры
* @return
*/
bool Camera::go()
{
  qInfo()<<"";

  //-- Запускаем камеру, если ещё не запущена
  if ( !start() ) { return false; }

  //-- Ждём, пока приконнектится //TODO: Заменить на QEventLoop
  while ( !(_status&S_CONNECTED) && !(_status&S_ERROR) ) { QCoreApplication::processEvents(); }

  //-- Подготавливаем камеру к вещанию
  if ( !setup() ) { return false; }

  //-- Ждём, пока поток камеры настраивается
  while ( !(_status&S_SETUPED) && !(_status&S_ERROR) ) { QCoreApplication::processEvents(); }

  //-- Запускаем вещание
  if ( !play() ) { return false; }

  //-- И ждём, пока начнётся
  while ( !(_status&S_PLAYED) && !(_status&S_ERROR) ) { QCoreApplication::processEvents(); }

  return !(_status&S_ERROR);
}

/**
* @brief Сбрасываем состояние
* @return
*/
bool Camera::reset()
{
  if ( _status&S_PLAYED ) { return stop(true); }
  if ( _status&S_ERROR ) { _status =S_NONE; return true; }
  return true;
}

/**
* @brief Начали приём видео
*/
void Camera::onPlayed()
{
  qInfo()<<"Camera played ";
  _status |=S_PLAYED;
  emit played();
}

/**
* @brief Закончили вещание
* @param chanelID
*/
void Camera::onTeardowned()
{
  if ( _status&S_ERROR ) { return; }
  qDebug()<<"Camera"<<id()<<"chanel"<<_channel<<"teardowned";
  _status |=S_NONE;
  _rtsp->cameraDisconnect();
  _clientsCount =0;
  emit teardowned();
}

/**
* @brief  Отдаём SDP медиа инфу о камере
* @return
*/
SDP::sMedia *Camera::getSDPMedia()
{
  return _rtsp->getChannel(_channel)->sdpMedia();
}

/**
* @brief Отдаём стример потока камеры
* @return
*/
NS_RSTP::IRTSPStream *Camera::getStreamer()
{
  return _rtsp->getChannel(_channel)->getStreamer();
}

Camera::~Camera()
{
  qDebug()<<"";
}
