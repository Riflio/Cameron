#ifndef RTSP_CHANNEL_H
#define RTSP_CHANNEL_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QTimer>

#include "sdp/sdp.h"
#include "rtsp/rtsp.h"
#include "rtsp/rtsp_stream.h"

namespace NS_RSTP {

/**
* @brief Канал вещания камеры
*/
class RTSP_Channel: public QObject
{
  Q_OBJECT

public:
  RTSP_Channel(RTSP * rtsp);
  ~RTSP_Channel();

  int id();
  long session();

  SDP::sMedia *sdpMedia();
  RTSP_Stream *getStreamer();

  friend class RTSP;

signals:
  void connected();
  void disconnected();
  void errored();
  void toTeardown(int channel); //-- Заканчиваем передачу

  void setuped(); //-- Создали канал для передачи
  void played(); //-- Начали передачу
  void teardowned(); //-- Закончили передачу
  void alived(); //-- Мы ещё живы и ведём передачу

public slots:
  void setup(int port);
  void play();
  void teardown();
  void alive();

private slots:
  void onStreamError();
  int onRTSPReply(RTSP::METHODS method, RTSP::THeaders headers);

private:
  RTSP *_rtsp =nullptr;
  int _id;
  long _session =0;
  SDP::sMedia *_sdpMedia =nullptr;
  QTimer *_aliveTimer =nullptr;
  QThread *_streamerThread =nullptr;
  RTSP_Stream *_streamer =nullptr;
  bool _alived =false;
};
}
#endif // RTSP_CHANNEL_H
