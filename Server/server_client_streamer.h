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
  uint32_t blockSize();
  void setBlockSize(uint32_t blockSize);

private slots:
  void onNewPacketAvaliable(QSharedPointer<IRTP_Packet> packet);

private:
  QHostAddress _host;
  int _port;
  int _id;

  IRTSP_Stream * _streamer =nullptr;
  QUdpSocket * _socket =nullptr;

  uint32_t _blockSize =1400; //-- Максимальный размер пакета, что бы влез в MTU
  uint16_t _sequence =0; //-- Будем использовать свою нумерацию пакетов //FIXME: Random init
};

#endif // SERVER_CLIENT_STREAMER_H
