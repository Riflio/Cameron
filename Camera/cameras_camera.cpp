#include "cameras_camera.h"



Cameras_Camera::Cameras_Camera(QObject *parent) : QObject(parent)
{
    qInfo()<<"Camera new";
    _status = S_NONE;
    _url = "";
    _clientsCount = 0;

}

int Cameras_Camera::status()
{
    return _status;
}

bool Cameras_Camera::setSettings(QString url, int id, int channel, int streamPort)
{
    qInfo()<<"Camera set settings "<<url;
    _id = id;
    _url = url;
    _channel = channel;
    _streamPort = streamPort;
    return true;
}


bool Cameras_Camera::start()
{
    if (_url=="") return false;

    if ( _status & S_STARTED) return true;

    qInfo()<<"Camera start ";
    _rtsp = new RTSP(this);

    _rtsp->cameraConnect(_url);

    connect(_rtsp, &RTSP::connected, this, &Cameras_Camera::onCameraConnected );
    connect(_rtsp, &RTSP::disconnected, this, &Cameras_Camera::onCameraDisconnected );

    connect(_rtsp, &RTSP::setuped, this, &Cameras_Camera::onSetuped );
    connect(_rtsp, &RTSP::played, this, &Cameras_Camera::onPlayed );

    _status |= S_STARTED;

    return true;
}

/**
 * @brief Удачно подключились к камере
 */
void Cameras_Camera::onCameraConnected()
{
    qInfo()<<"Camera connected ";     
    _status |= S_CONNECTED;
}

/**
 * @brief Отключились от камеры (сами или камера закрыла соединение)
 */
void Cameras_Camera::onCameraDisconnected()
{
    qInfo()<<"Camera disconnected";
    _status = S_NONE;
    _rtsp->deleteLater();
}

/**
 * @brief Подготавливаем камеру к вещанию
 * @return
 */
bool Cameras_Camera::setup()
{
    if (_status & S_SETUPED) return true;

    RTSP_Channel * channel = _rtsp->getChannel(_channel);

    if (channel==NULL) {
        qWarning()<<"Channel not found!";
        return false;
    }

    channel->setup(_streamPort);

    connect(channel, &RTSP_Channel::disconnected, this, &Cameras_Camera::onChannelDisconnected);

    return true;
}

/**
 * @brief Канал закрыл соединение или ошибка сети
 */
void Cameras_Camera::onChannelDisconnected()
{
    qInfo()<<"Channel disconnected";
    _status &= ~S_SETUPED;
    _status &= ~S_PLAYED;
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

    _clientsCount++;

    if ( _status & S_PLAYED ) return true; //-- нам незачем дважды запускать

    _rtsp->getChannel(_channel)->play();
    return true;
}

bool Cameras_Camera::stop()
{
    _clientsCount--;

    if ( (_status & S_PLAYED)  && _clientsCount<=0) { //-- если запущена и не осталось больше подключившихся
        _rtsp->getChannel(_channel)->teardown();
        _status &= ~S_PLAYED;
    }

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

/**
 * @brief  Отдаём SDP медиа инфу о камере
 * @return
 */
SDP::sMedia * Cameras_Camera::getSDPMedia()
{
    qInfo()<<"Camera getSDP";
    return _rtsp->getChannel(_channel)->sdpMedia();
}

/**
 * @brief Отдаём стример потока камеры
 * @return
 */
NS_RSTP::RTSP_Stream * Cameras_Camera::getStreamer()
{
    return _rtsp->getChannel(_channel)->getStreamer();
}
