#ifndef RTSP_STREAM_H
#define RTSP_STREAM_H


#include <QObject>
#include <QByteArray>
#include <QQueue>
#include <QMutexLocker>

#include "../rtp/rtp.h"


/**
 * @brief Общий интерфейс стримеров, сдесь собираем готовые фреймы из RTP пакетов в круговой буфер (старые фреймы затираются новыми)
 */

namespace NS_RSTP {
class RTSP_Stream: public RTP

{    
    Q_OBJECT
public:
    explicit RTSP_Stream(QObject * parent =0);

    const int MAX_FRAMES = 250;

    QByteArray getFrame(int & offset);

    int getOffset(int cur=0);



signals:
    void newFrame(QByteArray frame);

public slots:

protected:
    QList<QByteArray> _frames; //-- буфер размером MAX_FRAMES из обработанных фреймов
    long long _framesOffset;
    void toNewFrame(QByteArray);
private:
    QMutex _mutex;
};
}
#endif // RTSP_STREAM_H
