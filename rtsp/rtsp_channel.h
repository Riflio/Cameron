#ifndef RTSP_CHANNEL_H
#define RTSP_CHANNEL_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QTimer>


#include "../sdp/sdp.h"
#include "rtp/rtp.h"

#include "../h264/h264_stream_rtp.h"

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

    RTSP_Stream * streamer;


    friend class RTSP;

signals:
    void connected();
    void newReadedData(QByteArray data);

public slots:
    void onReadyRead();
    void setup(int port);
    void play();
    void teardown();
    void alive();


private:
    QUdpSocket * _socket;
    RTSP * _connect;
    int _id;
    int _port;
    long _session;
    SDP::sMedia * _sdpMedia;

    QTimer * _aliveTimer;

    QThread * thread;
};
}
#endif // RTSP_CHANNEL_H
