#include "rtsp_channel.h"
#include "rtsp.h"

#include <QDebug>
namespace NS_RSTP {

RTSP_Channel::RTSP_Channel(RTSP * parent):
    QObject(parent)
{
    _connect = parent;
    _socket = NULL;
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

    _port = port;
    _socket = new QUdpSocket(this);

    //-- подготавливаем стример
    //TODO: сделать обработку разными стримерами в зависимости от типа кодирования в DESCRIBE
    streamer = new NS_H264::H264_Stream_RTP(0);

    //-- засунем стример в новый поток, что бы не замедлял чтение UDP
    thread = new QThread(this);
    streamer->moveToThread(thread);
    connect(thread, SIGNAL(started()), streamer, SLOT(process()) );

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()) );
    connect(_socket, SIGNAL(connected()), this, SIGNAL(connected()) );

    _socket->bind(port);

    thread->start();

    _connect->setup(id(), port);

}

/**
 * @brief Начинаем воспроизведение
 */
void RTSP_Channel::play()
{
    qInfo()<<"RTSP_Channel play";



    //-- первым делом пишем базовые параметры
    //-- sps и pps хранятся в sprop-parameter-sets, разделённые запятушкой, закодированные в base64
    QStringList ps =  _sdpMedia->attribytes.value("fmtp")->parameters.value("sprop-parameter-sets").split(",");

    ((NS_H264::H264_Stream_RTP*)streamer)->setPS( ps.at(0), ps.at(1) );



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


/**
 * @brief Есть новые данные в потоке, передаём на парсинг в RTP
 */
void RTSP_Channel::onReadyRead()
{
    QByteArray data;

    data.resize(_socket->pendingDatagramSize());
    _socket->readDatagram(data.data(), data.size() );

    streamer->newPacket(data);

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


RTSP_Channel::~RTSP_Channel()
{

}

}
