#include "cameras.h"

Cameras::Cameras(QObject *parent) : QObject(parent)
{

}

/**
* @brief Создаём новую камеру
* @param id
* @return
*/
ICamera *Cameras::newCam(int id)
{
  Camera *cam =new Camera(this);
  _cams.insert(id, cam);
  return cam;
}

/**
* @brief Отдаём указанную камеру
* @param id
* @return
*/
ICamera *Cameras::getCam(int id)
{
  return _cams.value(id, nullptr);
}

/**
* @brief Собираем SDP со всех камер
* @param trackId - айдишник конкретного трека, или -1 когда не задан явно
* @return
*/
ISDP *Cameras::getTotalSDP(int trackId)
{
  TCams::iterator i;
  SDP *sdp =new SDP(this);
  for (i=_cams.begin(); i!=_cams.end(); ++i) {
    Camera *cam =static_cast<Camera*>(i.value());
    if ( trackId>=0 && cam->id()!=trackId ) { continue; }

    //-- Если камера в ошибке, то попробуем сбросить
    if ( cam->status()&Camera::S_ERROR ) { cam->reset(); }

    //-- Если камера ещё не запущена, то придётся ждать коннекта, иначе нам не получить SDP
    while ( !(cam->status()&Camera::S_CONNECTED) && !(cam->status()&Camera::S_ERROR) ) {
      if ( !(cam->status()&Camera::S_STARTED) ) { //-- Если ещё не запустили, запускаем
        if ( !cam->start() ) { break; }
      }
      QCoreApplication::processEvents();
    }

    //-- Если в итоге не удалось подключиться, пропускаем
    if ( !(cam->status()&Camera::S_CONNECTED) ) { continue; }

    SDP::sMedia *media =cam->getSDPMedia();
    media->attribytes.value("control")->value = (QString("track/%1").arg(cam->id())); //-- Подменим параметр управления на йдишник камеры, т.е. потока для уникальности среди потоков сервера.
    sdp->medias.append(media);
  }

  //-- Выставим базовые сведения SDP, что бы хоть что-то было
  SDP::sOrigin origin;
  origin.version =0;
  sdp->origin =origin;

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
