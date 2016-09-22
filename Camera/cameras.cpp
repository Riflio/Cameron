#include "cameras.h"
#include <QApplication>

Cameras::Cameras(QObject *parent) : QObject(parent)
{

}

/**
 * @brief Создаём новую камеру
 * @param id
 * @return
 */
Cameras_Camera * Cameras::newCam(int id)
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
Cameras_Camera * Cameras::getCam(int id)
{
    return _cams.value(id, NULL);
}

/**
 * @brief Собираем SDP со всех камер, подменяем айдишником камеры паарметр управления
 * @return
 */
SDP * Cameras::getTotalSDP()
{
    TCams::iterator i;

    SDP * sdp = new SDP(this);

    for (i = _cams.begin(); i != _cams.end(); ++i) {
        Cameras_Camera * cam = i.value();

        bool statusOK = true;
        while ( !(cam->status()&Cameras_Camera::S_CONNECTED) ) //-- придётся ждать коннекта, иначе нам не получить SDP
        {
           if ( !(cam->status()&Cameras_Camera::S_STARTED)) { //-- если ещё не запустили, запускаем
               if (!cam->start()) { statusOK=false; break; } //-- если не удастся подключиться, то пропускаем
           }
           QApplication::processEvents();
        }

        if (!statusOK) continue;

        SDP::sMedia * media = cam->getSDPMedia();

        media->attribytes.value("control")->value=(QString("trackID=%1").arg( i.key() )); //-- подменим параметр управления на йдишник камеры, что бы в дальнейшем знать какой клиент к какой камере относиться

        sdp->medias.append( media );

    }

    //-- Выставим базовые сведения SDP, что бы хоть что-то было
    SDP::sOrigin origin;
    origin.version=0;
    sdp->origin = origin;

    return sdp;
}

TCams Cameras::getCams()
{
    return _cams;
}

Cameras::~Cameras()
{
    qInfo()<<"Cameras deleted";
}
