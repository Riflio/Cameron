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
    return _cams.value(id, NULL);
}

/**
 * @brief Собираем SDP со всех камер, подменяем айдишником камеры параметр управления
 * @return
 */
ISDP * Cameras::getTotalSDP()
{
    TCams::iterator i;

    SDP * sdp = new SDP(this);

    for (i = _cams.begin(); i != _cams.end(); ++i) {
        Cameras_Camera * cam = static_cast<Cameras_Camera*>( i.value() );

        while ( !(cam->status()&Cameras_Camera::S_CONNECTED) && !(cam->status()&Cameras_Camera::S_ERROR) ) //-- придётся ждать коннекта, иначе нам не получить SDP
        {
           if ( !(cam->status()&Cameras_Camera::S_STARTED)) { //-- если ещё не запустили, запускаем
               if (!cam->start()) { break; }
           }
           QCoreApplication::processEvents();
        }

        if (!(cam->status()&Cameras_Camera::S_CONNECTED)) continue; //-- Не удалось подключиться, пропускаем

        SDP::sMedia * media = cam->getSDPMedia();

        media->attribytes.value("control")->value=(QString("trackID=%1").arg( i.key() )); //-- подменим параметр управления на йдишник камеры, что бы в дальнейшем знать какой клиент к какой камере относится

        sdp->medias.append( media );

    }

    //-- Выставим базовые сведения SDP, что бы хоть что-то было
    SDP::sOrigin origin;
    origin.version=0;
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
