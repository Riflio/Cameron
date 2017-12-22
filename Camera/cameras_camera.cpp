#include "cameras_camera.h"

Cameras_Camera::Cameras_Camera(QObject *parent) : QObject(parent)
{
    qInfo()<<"Camera new";
    _status = S_NONE;
    _url = "";
    _rtsp = NULL;
    _clientsCount = 0;

}

int Cameras_Camera::id()
{
    return _id;
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
    connect(_rtsp, &RTSP::connected, this, &Cameras_Camera::onCameraConnected );
    connect(_rtsp, &RTSP::disconnected, this, &Cameras_Camera::onCameraDisconnected );
    connect(_rtsp, &RTSP::errored, this, &Cameras_Camera::onCameraErrored);

    connect(_rtsp, &RTSP::setuped, this, &Cameras_Camera::onSetuped );
    connect(_rtsp, &RTSP::played, this, &Cameras_Camera::onPlayed );
    connect(_rtsp, &RTSP::toTeardown, this, &Cameras_Camera::onTeardowned);

    _rtsp->cameraConnect(_url);

    _status &= ~S_ERROR; //-- Уберём ошибку в случае попытки переподключения
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
    emit Events->doAction("CameraConnected", QVariantList()<<Events->ARG(this));
}

/**
 * @brief Отключились от камеры (сами или камера закрыла соединение)
 */
void Cameras_Camera::onCameraDisconnected()
{
    qInfo()<<"Camera disconnected";
    _status = S_NONE;
    _rtsp->deleteLater();
    emit Events->doAction("CameraDisconnected", QVariantList()<<Events->ARG(this));
}

/**
* @brief Возникли ошибки при общении с камерой
*/
void Cameras_Camera::onCameraErrored()
{    
    _rtsp->deleteLater();
    _status = S_NONE;
    _status |= S_ERROR;
    emit Events->doAction("CameraErrored", QVariantList()<<Events->ARG(this));
    emit errored();
}

/**
 * @brief Подготавливаем камеру к вещанию
 * @return
 */
bool Cameras_Camera::setup()
{
    if (_status & S_SETUPED) return true;
    if (_status & S_ERROR) return false;

    qInfo()<<"";

    RTSP_Channel * channel = _rtsp->getChannel(_channel);

    if (channel==NULL) {
        qWarning()<<"Channel not found!"<<_channel;
        return false;
    }

    channel->setup(_streamPort);

    return true;
}

/**
* @brief Удачно выставили настройки
*/
void Cameras_Camera::onSetuped(int)
{
    qInfo()<<"Camera setuped";
    _status |= S_SETUPED;
    emit Events->doAction("CameraSetuped", QVariantList()<<Events->ARG(this));
}

/**
 * @brief Запускаем передачу видео
 * @return
 */
bool Cameras_Camera::play()
{
    qInfo()<<"Camera play";

    if (_status & S_ERROR) return false;

    _clientsCount++;

    if ( _status & S_PLAYED ) return true; //-- нам незачем дважды запускать

    _rtsp->getChannel(_channel)->play();
    return true;
}

bool Cameras_Camera::stop()
{
    qInfo()<<"Camera stop";
    _clientsCount--;

    if ( (_status & S_PLAYED)  && _clientsCount<=0) { //-- Если запущена и не осталось больше подключившихся
        _rtsp->getChannel(_channel)->teardown();        
        _status = S_NONE;
    }

    return true;
}

/**
* @brief Делаем всё, что бы запустить вещание камеры
* @return
*/
bool Cameras_Camera::go()
{
    qInfo()<<"";

    //-- запускаем камеру, если ещё не запущена
    if (!start()) return false;

    //-- ждём, пока приконнектится
    while ( !(_status & S_CONNECTED) && !(_status & S_ERROR) ) { QApplication::processEvents(); }

    //-- подготавливаем камеру к вещанию
    if (!setup()) return false;

    //-- ждём, пока поток камеры настраивается
    while ( !(_status & S_SETUPED) && !(_status & S_ERROR) ) { QApplication::processEvents(); }

    //-- запускаем вещание
    if (!play()) return false;

    //-- и ждём, пока начнётся
    while ( !(_status & S_PLAYED) && !(_status & S_ERROR) ) { QApplication::processEvents(); }

    return !(_status & S_ERROR);
}

/**
 * @brief Начали приём видео
 */
void Cameras_Camera::onPlayed(int)
{
    qInfo()<<"Camera played ";    
    _status |= S_PLAYED;
    emit Events->doAction("CameraPlayed", QVariantList()<<Events->ARG(this));
}

/**
* @brief Закончили вещание
* @param chanelID
*/
void Cameras_Camera::onTeardowned(int chanelID)
{
    Q_UNUSED(chanelID);
    _status |= S_NONE;
    _rtsp->deleteLater();
}

/**
 * @brief  Отдаём SDP медиа инфу о камере
 * @return
 */
SDP::sMedia * Cameras_Camera::getSDPMedia()
{
    qDebug()<<"Camera getSDP";
    return _rtsp->getChannel(_channel)->sdpMedia();
}

/**
 * @brief Отдаём стример потока камеры
 * @return
 */
NS_RSTP::IRTSP_Stream * Cameras_Camera::getStreamer()
{
    return _rtsp->getChannel(_channel)->getStreamer();
}


Cameras_Camera::~Cameras_Camera()
{
    qDebug()<<"";
}
