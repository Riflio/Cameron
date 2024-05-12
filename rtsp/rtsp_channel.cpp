#include "rtsp_channel.h"
#include "rtsp.h"
#include "estd/estd.h"
#include <QDebug>

namespace NS_RSTP {

RTSP_Channel::RTSP_Channel(RTSP * rtsp): QObject(rtsp), _rtsp(rtsp), _id(_rtsp->channelsCount())
{
  qDebug()<<"RTSP_Channel created!";

  _streamer =new RTSP_Stream();
  _streamerThread =new QThread(this);
  _streamer->moveToThread(_streamerThread);

  //-- Как только поток запустится запускаем стример
  connect(_streamerThread, &QThread::started, _streamer, &RTSP_Stream::start);
  //-- Как только стример сам пожелает завершится завершаем сначала поток
  connect(_streamer, &RTSP_Stream::completed, _streamerThread, &QThread::quit);

  connect(_streamer, &RTSP_Stream::connected, this, &RTSP_Channel::connected, Qt::QueuedConnection);
  connect(_streamer, &RTSP_Stream::disconnected, this, &RTSP_Channel::disconnected, Qt::QueuedConnection);
  connect(_streamer, &RTSP_Stream::errored, this, &RTSP_Channel::onStreamError, Qt::QueuedConnection);

  _aliveTimer =new QTimer(this);
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

  RTSP::THeaders headers;
  headers.append(qMakePair("Transport", QString("RTP/AVP;unicast;client_port=%1-%2").arg(port).arg(port+1).toUtf8()));
  int r =_rtsp->send(RTSP::SETUP, headers, id());
  if ( r!=0 ) { emit errored(); return; }
}

/**
* @brief Начинаем воспроизведение
*/
void RTSP_Channel::play()
{
  qInfo()<<"play";
  if ( _aliveTimer->isActive() ) { return; } //-- Мы уже запущены
  _alived = true;
  _aliveTimer->start();
  _streamerThread->start(QThread::NormalPriority);

  RTSP::THeaders headers;
  headers.append(qMakePair("Session", QString::number(session()).toUtf8()));
  headers.append(qMakePair("Range", "npt=0.000-"));
  int r =_rtsp->send(RTSP::PLAY, headers, id());
  if ( r!=0 ) { emit errored(); return; }
}

/**
* @brief Отсылаем запрос как подтверждение, что мы ещё живы
*/
void RTSP_Channel::alive()
{
  qInfo()<<"alive";

  //-- Если с момента предыдущего запроса не было подтверждения ответа, то значит, что умерли (сеть или хз)
  if ( !_alived ) { _aliveTimer->stop(); emit errored(); return; }
  _alived =false;

  RTSP::THeaders headers;
  headers.append(qMakePair("Session", QString::number(session()).toUtf8()));
  int r =_rtsp->send(RTSP::ALIVE, headers, id());
  if ( r!=0 ) { emit errored(); return; }
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
  if ( _streamerThread->isRunning() ) {
    _streamerThread->quit();
    _streamerThread->wait();
  }

  //-- Если мы не запущены (ошибка или вообще не запускались), дальше делать нечего
  if ( !_aliveTimer->isActive() ) { return; }
  qInfo()<<"teardown";

  _aliveTimer->stop();  

  RTSP::THeaders headers;
  headers.append(qMakePair("Session", QString::number(session()).toUtf8()));
  int r =_rtsp->send(RTSP::TEARDOWN, headers, id());
  if ( r!=0 ) { emit errored(); return; }
  emit toTeardown(id());
}

/**
* @brief Пришёл ответ на запрос
* @param method
* @param headers
*/
int RTSP_Channel::onRTSPReply(RTSP::METHODS method, RTSP::THeaders headers)
{
  switch (method) {
    case RTSP::SETUP: {
      //-- От ответа, пока что, нам нужен номер сессии
      auto sessionIt =std::find_if(headers.begin(), headers.end(), CompareFirst<QByteArray, QByteArray>("session"));
      if ( sessionIt==headers.end() ) { qWarning()<<"Has no session value!"; return -1; }
      QList<QByteArray> sessionInfo =sessionIt->second.split(';');
      _session =sessionInfo.at(0).toInt();
      emit setuped();
      break;
    }
    case RTSP::PLAY: {
      emit played();
      break;
    }
    case RTSP::TEARDOWN: {
      emit teardowned();
      break;
    }
    case RTSP::GET_PARAMETER:
    case RTSP::ALIVE: {
      _alived =true;
      emit alived();
      break;
    }
    default: { return -2; } //-- Мы другого пока что не умеем
  }

  return 0;
}

int RTSP_Channel::id()
{
  return _id;
}

long RTSP_Channel::session()
{
  return _session;
}

SDP::sMedia *RTSP_Channel::sdpMedia()
{
  return _sdpMedia;
}

RTSP_Stream *RTSP_Channel::getStreamer()
{
  return _streamer;
}

RTSP_Channel::~RTSP_Channel()
{
  teardown();
  _streamerThread->deleteLater();
  delete _streamer; //-- Поток уже остановлен, так что сразу удаляемся
  qDebug()<<"";
}

}
