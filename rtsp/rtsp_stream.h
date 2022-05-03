#ifndef RTSP_STREAM_H
#define RTSP_STREAM_H


#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "Interfaces/irtp_packet.h"
#include "Plugins/wthread.h"
#include "Interfaces/irtsp_stream.h"

namespace NS_RSTP {

/**
* @brief Общий для стримеров, сдесь собираем готовые фреймы из RTP пакетов
*/
class RTSP_Stream: public WThread, public virtual IRTSP_Stream
{    
  Q_OBJECT
public:
  explicit RTSP_Stream(QObject * parent);
  ~RTSP_Stream();

  void setPort(int port);

  void loop() override;
  bool onStarted() override;
  void onFinished() override;


signals:
  void connected();
  void disconnected();
  void errored();
  void newPacketAvaliable(QSharedPointer<IRTP_Packet> packet) override;

private slots:
  void goError();
  void onReadyRead();

private:
  int _port;
  QUdpSocket * _socket;

};

}
#endif // RTSP_STREAM_H
