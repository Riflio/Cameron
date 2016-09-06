#ifndef RIPPER_H
#define RIPPER_H

#include <QObject>
#include <QFile>
#include <QDateTime>

#include "h264/h264_stream_rtp.h"
#include "rtsp/rtsp_stream.h"

using namespace NS_RSTP;
using namespace NS_H264;

class Ripper : public QObject
{
    Q_OBJECT
public:
    explicit Ripper(RTSP_Stream * streamer, QString folder);
    ~Ripper();

    QString savePath();

signals:
    void complete();

public slots:
    void process();
    void stop();

private:
    bool _toStop; //-- что бы знать, когда нужно остановиться
    QString _folder;
    RTSP_Stream * _streamer;
    int _framesOffset;
    QString _savePath;
};

#endif // RIPPER_H
