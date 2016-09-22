#include "rtsp_channel.h"
#include "rtsp.h"

#include <QDebug>
namespace NS_RSTP {

RTSP_Channel::RTSP_Channel(RTSP * parent):
    QObject(parent)
{
    _connect = parent;
    _id = _connect->channelsCount();
    _session = 0;

    _aliveTimer = new QTimer(this);
    _aliveTimer->setInterval(50000);

    connect(_aliveTimer, SIGNAL(timeout()), this, SLOT(alive()) );
}

/**
 * @brief Устанавливаем соединение
 * @param port
 */
void RTSP_Channel::setup(int port)
{
    qInfo()<<"RTSP_Channel setup"<<port;

    //-- подготавливаем стример
    _streamer = new RTSP_Stream(this, port);
    connect(_streamer, &RTSP_Stream::connected, this, &RTSP_Channel::connected);
    connect(_streamer, &RTSP_Stream::disconnected, this, &RTSP_Channel::disconnected);


    _connect->setup(id(), port);

}

/**
 * @brief Начинаем воспроизведение
 */
void RTSP_Channel::play()
{
    qInfo()<<"RTSP_Channel play";

    _streamer->start();

    _aliveTimer->start();

    _connect->play(id());
}


void RTSP_Channel::alive()
{
    qInfo()<<"RTSP_Channel alive";
    _connect->alive(id());
}

void RTSP_Channel::teardown()
{
    qInfo()<<"RTSP_Channel teardown";

    _aliveTimer->stop();
    _connect->teardown(id());
}


int RTSP_Channel::id()
{
    return _id;
}

long RTSP_Channel::session()
{
    return _session;
}

SDP::sMedia * RTSP_Channel::sdpMedia()
{
    return _sdpMedia;
}

RTSP_Stream * RTSP_Channel::getStreamer()
{
    return _streamer;
}


RTSP_Channel::~RTSP_Channel()
{
    qInfo()<<"RTSP channel deleted";
}

}
