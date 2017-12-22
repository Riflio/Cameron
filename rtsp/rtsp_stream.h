#ifndef RTSP_STREAM_H
#define RTSP_STREAM_H


#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "../Assets/wthread.h"
#include "../Interfaces/irtsp_stream.h"
#include "../rtp/rtp.h"


/**
 * @brief Общий для стримеров, сдесь собираем готовые фреймы из RTP пакетов
 */

namespace NS_RSTP {

class RTSP_Stream: public WThread, public RTP, public IRTSP_Stream
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
    void errored();

private slots:
    void goError();

private:
    int _port;
    QUdpSocket * _socket;

    QTimer * _processTimer;
    void processLoop();
};

}
#endif // RTSP_STREAM_H
