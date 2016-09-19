#ifndef RTSP_STREAM_H
#define RTSP_STREAM_H


#include <QObject>
#include <QUdpSocket>
#include <QThread>

#include "../rtp/rtp.h"


/**
 * @brief Общий интерфейс стримеров, сдесь собираем готовые фреймы из RTP пакетов
 */

namespace NS_RSTP {

class RTSP_Stream: public RTP

{    
    Q_OBJECT
public:
    explicit RTSP_Stream(QObject * parent, int port);

    void process();


signals:
    void finished();
    void connected();
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
