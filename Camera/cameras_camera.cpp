#include "cameras_camera.h"



Cameras_Camera::Cameras_Camera(QObject *parent) : QObject(parent)
{
    qInfo()<<"Camera new";
    _status = S_NONE;
    _url = "";
    _rtsp = new RTSP(this);


}

int Cameras_Camera::status()
{
    return _status;
}

bool Cameras_Camera::setSettings(QString url, int id)
{
    qInfo()<<"Camera set settings "<<url;
    _id = id;
    _url = url;

}


bool Cameras_Camera::start()
{
    if (_url=="") return false;
    qInfo()<<"Camera start ";

    _rtsp->cameraConnect(_url);

    connect(_rtsp, SIGNAL(connected()), this, SLOT(onCameraConnected()) );
    connect(_rtsp, SIGNAL(setuped(int)), this, SLOT(onSetuped(int)) );
    connect(_rtsp, SIGNAL(played(int)), this, SLOT(onPlayed(int)) );

    _status |= S_STARTED;

    return true;
}

/**
 * @brief Удачно подключились к камере, выставляем настройки
 */
void Cameras_Camera::onCameraConnected()
{
    qInfo()<<"Camera connected ";
    _rtsp->getChannel(0)->setup(4041);

    _status |= S_CONNECTED;
}

/**
 * @brief Удачно выставили настройки
 */
void Cameras_Camera::onSetuped(int)
{
    qInfo()<<"Camera setuped ";
    _status |= S_SETUPED;
}

/**
 * @brief Запускаем передачу видео
 * @return
 */
bool Cameras_Camera::play()
{
    qInfo()<<"Camera play";
     _rtsp->getChannel(0)->play();
     return true;
}

bool Cameras_Camera::stop()
{
    return true;
}

/**
 * @brief Начали приём видео
 */
void Cameras_Camera::onPlayed(int)
{
    qInfo()<<"Camera played ";
    _status |= S_PLAYED;

}

SDP::sMedia * Cameras_Camera::getSDPMedia()
{
    qInfo()<<"Camera getSDP";
    return _rtsp->getChannel(0)->sdpMedia();
}

