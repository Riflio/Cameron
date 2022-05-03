#include "rtsp_channel.h"
#include "rtsp.h"

#include <QDebug>
namespace NS_RSTP {

RTSP_Channel::RTSP_Channel(QObject * parent, RTSP * rtsp)
    : QObject(parent), _connect(rtsp), _id(_connect->channelsCount()), _session(0), _streamer(nullptr), _alived(false)
{
  qDebug()<<"RTSP_Channel created!";
  _streamer = new RTSP_Stream(this);
  connect(_streamer, &RTSP_Stream::connected, this, &RTSP_Channel::connected);
  connect(_streamer, &RTSP_Stream::disconnected, this, &RTSP_Channel::disconnected);
  connect(_streamer, &RTSP_Stream::errored, this, &RTSP_Channel::onStreamError);

  _aliveTimer = new QTimer(this);
  _aliveTimer->setInterval(50000);
  connect(_aliveTimer, &QTimer::timeout, this, &RTSP_Channel::alive);
}

/**
* @brief Устанавливаем соединение
* @param port
*/
void RTSP_Channel::setup(int port)
{
  qInfo()<<"setup"<<port;
  _streamer->setPort(port);
  _connect->setup(id(), port);
}

/**
* @brief Начинаем воспроизведение
*/
void RTSP_Channel::play()
{
  qInfo()<<"play";
  if ( _aliveTimer->isActive() ) { return; } //-- Мы уже запущены
  _streamer->start(QThread::HighPriority);
  _alived = true;
  _aliveTimer->start();
  _connect->play(id());
}

/**
* @brief Отсылаем запрос, что мы ещё живы
*/
void RTSP_Channel::alive()
{
  qInfo()<<"alive";
  if ( !_alived ) { //-- Если с момента предыдущего запроса не было подтверждения ответа, то значит, что умерли (сеть или хз)
    _aliveTimer->stop();
    emit errored();
    return;
  }
  _alived = false;
  _connect->alive(id());
}

/**
* @brief Пришло подтверждение ответа на запрос о том, что мы живы
*/
void RTSP_Channel::alived()
{
  _alived = true;
}

/**
* @brief Возникла ошибка с принимающим потоком
*/
void RTSP_Channel::onStreamError()
{
  qDebug()<<"";
  emit errored();
}

/**
* @brief Отсылаем запрос на завершение вещания
*/
void RTSP_Channel::teardown()
{    
  if ( !_aliveTimer->isActive() ) { return; } //-- Если мы не были запущены
  qInfo()<<"teardown";

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
  teardown();
  qDebug()<<"";
}

}
