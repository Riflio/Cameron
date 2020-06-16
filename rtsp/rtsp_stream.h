#ifndef RTSP_STREAM_H
#define RTSP_STREAM_H


#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "Plugins/wthread.h"
#include "Interfaces/irtsp_stream.h"
#include "rtp/rtp.h"

namespace NS_RSTP {

/**
* @brief Общий для стримеров, сдесь собираем готовые фреймы из RTP пакетов
*/
class RTSP_Stream: public WThread, public RTP, public IRTSP_Stream
{    
    Q_OBJECT
public:
    explicit RTSP_Stream(QObject * parent, int port);
    ~RTSP_Stream();

    void loop() override;
    bool onStarted() override;


signals:
    void connected();
    void disconnected();
    void errored();

private slots:
    void goError();

private:
    int _port;
    QUdpSocket * _socket;

};

}
#endif // RTSP_STREAM_H
