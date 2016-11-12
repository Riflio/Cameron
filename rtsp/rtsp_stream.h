#ifndef RTSP_STREAM_H
#define RTSP_STREAM_H


#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QApplication>

#include "../Interfaces/irtsp_stream.h"
#include "../rtp/rtp.h"


/**
 * @brief Общий для стримеров, сдесь собираем готовые фреймы из RTP пакетов
 */

namespace NS_RSTP {

class RTSP_Stream: public QObject, public RTP, public IRTSP_Stream
{    
    Q_OBJECT
public:
    explicit RTSP_Stream(QObject * parent, int port);
    ~RTSP_Stream();

    void process();


signals:
    void finished();
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);

public slots:
    bool start();
    void stop();

private:
    int _port;
    QUdpSocket * _socket;
    bool _started;
    QThread * _thread;
};

}
#endif // RTSP_STREAM_H
