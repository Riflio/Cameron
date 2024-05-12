#ifndef RTSP_STREAM_H
#define RTSP_STREAM_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "interfaces/irtp_packet.h"
#include "interfaces/irtsp_stream.h"

namespace NS_RSTP {

/**
* @brief Общий для стримеров, сдесь собираем готовые фреймы из RTP пакетов
*/
class RTSP_Stream: public QObject, public virtual IRTSP_Stream
{    
  Q_OBJECT
public:
  explicit RTSP_Stream(QObject * parent =nullptr);
  ~RTSP_Stream();

  void setPort(int port);

  void onFinished();

signals:
  void completed();
  void connected();
  void disconnected();
  void errored();
  void newPacketAvaliable(QSharedPointer<IRTP_Packet> packet) override;

public slots:
  void start();

private slots:
  void goError();
  void onReadyRead();

private:
  int _port =0;
  QUdpSocket *_socket =nullptr;

};

}
#endif // RTSP_STREAM_H
