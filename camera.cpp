#include "camera.h"

Camera::Camera(QObject *parent) : QObject(parent)
{
    qInfo()<<"Camera new";
    _url = "";

    _rtsp = new RTSP(this);

    _ripTimer = new QTimer(this);
    _ripTimer->setSingleShot(true);

    connect(_ripTimer, SIGNAL(timeout()), this, SLOT(ripStop()));
}


bool Camera::setSettings(QString url, int id, int ripSecconds, QString savePath)
{
    qInfo()<<"Camera set settings "<<url<<ripSecconds;
    _id = id;
    _url = url;
    _ripSecconds = ripSecconds;
    _savePath = savePath;
}


bool Camera::start()
{
    if (_url=="") return false;
    qInfo()<<"Camera start ";

    _rtsp->cameraConnect(_url);

    connect(_rtsp, SIGNAL(connected()), this, SLOT(onCameraConnected()) );
    connect(_rtsp, SIGNAL(setuped(int)), this, SLOT(onSetuped(int)) );
    connect(_rtsp, SIGNAL(played(int)), this, SLOT(onPlayed(int)) );

    return true;
}

/**
 * @brief Удачно подключились к камере, выставляем настройки
 */
void Camera::onCameraConnected()
{
    qInfo()<<"Camera connected ";
    _rtsp->getChannel(0)->setup(4041);
}

/**
 * @brief Удачно выставили настройки, запускаем передачу видео
 */
void Camera::onSetuped(int)
{
    qInfo()<<"Camera setuped ";
    _rtsp->getChannel(0)->play();
}

/**
 * @brief Начали приём видео,настраиваем декодер
 */
void Camera::onPlayed(int)
{
    qInfo()<<"Camera played ";

    _decoder = new H264_Decoder(0, _rtsp->getChannel(0)->streamer);

    QThread * th = new QThread(this);

    _decoder->moveToThread(th);

    if (_decoder->init()) {

        _motionDetect = new MotionDetect(0);

        connect(_decoder, SIGNAL(newDecodedFrameData(QSharedPointer<uint8_t>,int,int)), _motionDetect, SLOT(detect(QSharedPointer<uint8_t>,int,int)) );

        connect(_motionDetect, SIGNAL(detected(int)), this, SLOT(onMotionDetect(int)) );

        connect(th, SIGNAL(started()), _decoder, SLOT(process()) );
        th->start();

    }
}

/**
 * @brief Было обнаружено движение
 */
void Camera::onMotionDetect(int count)
{
    if ( !_ripTimer->isActive() ) { //-- если нет записи, то запускаем
        _ripTimer->start(_ripSecconds*1000);
        ripStart();
    } else { //-- добавим ещё времени к записи
        _ripTimer->setInterval( _ripSecconds*1000);
    }

}



void Camera::ripStart()
{
    qInfo()<<"RIP Start";

    _ripThread = new QThread(this);
    _ripper = new Ripper(_rtsp->getChannel(0)->streamer, _savePath);

    connect(_ripThread, &QThread::started, _ripper, &Ripper::process ); //-- при запуске потока сразу начинаем работу
    connect( _ripper, &Ripper::complete, _ripThread, &QThread::quit ); //-- как только вся работа сделана или остановлена, останавливаем поток
    connect(_ripThread, &QThread::finished, _ripper, &Ripper::deleteLater); //-- как только поток закончен удаляемся
    connect(_ripper, &Ripper::destroyed, [=](){ _ripper = NULL; _ripThread->deleteLater(); } ); //-- Как только удалились, выставляем указатель в ноль, что бы потом случайно не заюзать и удаляем сам поток

    _ripper->moveToThread(_ripThread);
    _ripThread->start();

    QSqlQuery query;
    query.exec( QString("INSERT INTO cameron_rips SET crr_camid=%1, crr_dt=\"%2\", crr_path=\"%3\", crr_action=\"%4\"").arg(_id).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg( _ripper->savePath() ).arg("move") );
    qInfo()<<"Last query"<<query.lastQuery();

}

void Camera::ripStop()
{
    qInfo()<<"RIP Stop";

    if (_ripper!=NULL) _ripper->stop();


}
