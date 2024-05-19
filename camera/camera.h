#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "rtsp/rtsp.h"
#include "interfaces/iqcamera.h"
#include "rtsp/rtspchannel.h"

using namespace NS_RSTP;

/**
* @brief Конкретная камера, точнее её один поток.
* Если у камеры несколько потоков, то каждый должен быть представлен камерой в настройках
*/
class Camera: public IQCamera
{
    Q_OBJECT
public:
  explicit Camera(QObject *parent =nullptr);
  virtual ~Camera();

  bool setSettings(QString url ="", int id =-1, int channel =0, int streamPort =4041) override;

  QString url() override { return _url; }

  int id() override;
  int status() override;

  ISDP::sMedia *getSDPMedia() override;
  IRTSPStream *getStreamer() override;

public slots:
  bool start() override;
  bool setup() override;
  bool play() override;
  bool stop(bool force =false) override;
  bool go() override;
  bool reset() override;

protected slots:
  void onCameraConnected();
  void onCameraDisconnected();
  void onCameraErrored();

  void onSetuped();
  void onPlayed();
  void onTeardowned();

private:
  int _id =-1;
  QString _url ="";
  int _streamPort =-1;
  int _channel =-1;
  RTSP *_rtsp =nullptr;
  int _status =S_NONE;
  int _clientsCount =0;

};

#endif // CAMERA_H
