#include "cameras.h"

Cameras::Cameras(QObject *parent) : QObject(parent)
{

}

/**
* @brief Создаём новую камеру
* @param id
* @return
*/
ICameras_Camera * Cameras::newCam(int id)
{
  Cameras_Camera * cam = new Cameras_Camera(this);
  _cams.insert(id, cam);
  return cam;
}

/**
* @brief Отдаём указанную камеру
* @param id
* @return
*/
ICameras_Camera * Cameras::getCam(int id)
{
  return _cams.value(id, nullptr);
}

/**
* @brief Собираем SDP со всех камер
* @param trackId - айдишник конкретного трека, или -1 когда не задан явно
* @return
*/
ISDP * Cameras::getTotalSDP(int trackId)
{
  TCams::iterator i;
  SDP * sdp = new SDP(this);
  for (i=_cams.begin(); i!=_cams.end(); ++i) {
    Cameras_Camera * cam = static_cast<Cameras_Camera*>(i.value());
    if ( trackId>=0 && cam->id()!=trackId ) { continue; }

    while ( !(cam->status()&Cameras_Camera::S_CONNECTED) && !(cam->status()&Cameras_Camera::S_ERROR) ) { //-- Придётся ждать коннекта, иначе нам не получить SDP
      if ( !(cam->status()&Cameras_Camera::S_STARTED) ) { //-- Если ещё не запустили, запускаем
        if ( !cam->start() ) { break; }
      }
      QCoreApplication::processEvents();
    }

    if ( !(cam->status()&Cameras_Camera::S_CONNECTED) ) { continue; } //-- Не удалось подключиться, пропускаем
    SDP::sMedia * media = cam->getSDPMedia();
    media->attribytes.value("control")->value = (QString("track/%1").arg(cam->id())); //-- Подменим параметр управления на йдишник камеры, т.е. потока для уникальности среди потоков сервера.
    sdp->medias.append(media);
  }

  //-- Выставим базовые сведения SDP, что бы хоть что-то было
  SDP::sOrigin origin;
  origin.version = 0;
  sdp->origin = origin;

  return dynamic_cast<ISDP*>(sdp);
}

TCams Cameras::getCams()
{
  return _cams;
}

Cameras::~Cameras()
{
  qDebug()<<"";
}
