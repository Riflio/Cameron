#ifndef RTSPSTREAM_H
#define RTSPSTREAM_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "interfaces/irtppacket.h"
#include "interfaces/iqrtspstream.h"

namespace NS_RSTP {

/**
* @brief Общий для стримеров, сдесь собираем готовые фреймы из RTP пакетов
*/
class RTSPStream: public IQRTSPStream
{    
  Q_OBJECT
public:
  explicit RTSPStream(QObject *parent =nullptr);
  ~RTSPStream();

  void setPort(int port);

  void onFinished();

signals:
  void completed();
  void connected();
  void disconnected();
  void errored();

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
#endif // RTSPSTREAM_H
