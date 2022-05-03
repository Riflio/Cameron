#ifndef CAMERAS_CAMERA_H
#define CAMERAS_CAMERA_H

#include <QObject>
#include "rtsp/rtsp.h"
#include "Interfaces/icameras_camera.h"
#include "Plugins/PluginEventsWrapper.h"

using namespace NS_RSTP;

/**
* @brief Конкретная камера, точнее её один поток.
* Если у камеры несколько потоков, то каждый должен быть представлен камерой в настройках
*/
class Cameras_Camera : public QObject, public ICameras_Camera
{
    Q_OBJECT
public:
  explicit Cameras_Camera(QObject *parent = nullptr);
  virtual ~Cameras_Camera();

  bool setSettings(QString url ="", int id =-1, int channel =0, int streamPort =4041);

  QString url() { return _url; }

  int id();
  int status();
  bool start();
  bool setup();
  bool play();
  bool stop();
  bool go();

  ISDP::sMedia * getSDPMedia();
  IRTSP_Stream * getStreamer();

signals:
  void errored();
  void connected();
  void disconnected();
  void setuped();
  void played();
  void teardowned();

protected slots:
  void onCameraConnected();
  void onCameraDisconnected();
  void onCameraErrored();

  void onSetuped(int);
  void onPlayed(int);
  void onTeardowned(int);

private:
  int _id;
  QString _url;
  int _streamPort;
  int _channel;
  RTSP * _rtsp;
  int _status;
  int _clientsCount;

};

#endif // CAMERAS_CAMERA_H
