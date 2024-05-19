#ifndef SERVERCLIENTSTREAMER_H
#define SERVERCLIENTSTREAMER_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>

#include "camera/camera.h"
#include "interfaces/irtppacket.h"

#include <QTimer>

/**
* @brief Отсылаем клиенту сервера фреймы
*/
class ServerClientStreamer: public QObject
{
    Q_OBJECT
public:
  explicit ServerClientStreamer(QObject *parent, QHostAddress host, int port, int id, IRTSPStream *streamer);
  ~ServerClientStreamer();

  int id();
  uint32_t blockSize();
  void setBlockSize(uint32_t blockSize);

signals:
  void completed();

public slots:
  void start();

private slots:
  void onNewPacketAvaliable(QSharedPointer<IRTPPacket> packet);

private:
  QHostAddress _host;
  int _port;
  int _id;

  IRTSPStream *_streamer =nullptr;
  QUdpSocket *_socket =nullptr;

  uint32_t _blockSize =1400; //-- Максимальный размер пакета, что бы влез в MTU
  uint16_t _sequence =0; //-- Будем использовать свою нумерацию пакетов //FIXME: Random init
};

#endif // SERVERCLIENTSTREAMER_H
