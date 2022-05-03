#ifndef SERVER_CLIENT_STREAMER_H
#define SERVER_CLIENT_STREAMER_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>

#include "Plugins/wthread.h"
#include "Camera/cameras_camera.h"
#include "Interfaces/irtp_packet.h"

#include <QTimer>

/**
* @brief Отсылаем клиенту сервера фреймы
*/
class Server_Client_Streamer : public WThread
{
    Q_OBJECT
public:
  explicit Server_Client_Streamer(QObject * parent, QHostAddress host, int port, int id,  IRTSP_Stream * streamer);
  ~Server_Client_Streamer();

  void loop() override;
  bool onStarted() override;

  int id();

private slots:
  void onNewPacketAvaliable(QSharedPointer<IRTP_Packet> packet);

private:
  QHostAddress _host;
  int _port;
  int _id;

  IRTSP_Stream * _streamer =nullptr;
  QUdpSocket * _socket =nullptr;
};

#endif // SERVER_CLIENT_STREAMER_H
