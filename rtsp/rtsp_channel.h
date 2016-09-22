#ifndef RTSP_CHANNEL_H
#define RTSP_CHANNEL_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QTimer>


#include "../sdp/sdp.h"
#include "rtp/rtp.h"

#include "rtsp/rtsp_stream.h"

namespace NS_RSTP {
class RTSP;
class RTSP_Channel: public QObject
{
    Q_OBJECT
public:
    RTSP_Channel(RTSP * parent);
    ~RTSP_Channel();


    int id();
    long session();

    SDP::sMedia * sdpMedia();

    RTSP_Stream * getStreamer();



    friend class RTSP;

signals:
    void connected();
    void disconnected();

public slots:
    void setup(int port);
    void play();
    void teardown();
    void alive();


private:
    RTSP * _connect;
    int _id;    
    long _session;
    SDP::sMedia * _sdpMedia;
    QTimer * _aliveTimer;
    RTSP_Stream * _streamer;

};
}
#endif // RTSP_CHANNEL_H
