#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include "rtsp/rtsp.h"
#include "h264/h264_decoder.h"
#include "Rip/ripper.h"

#include <QtSql>

using namespace NS_RSTP;
using namespace NS_H264;
#include "OpenCV/motiondetect.h"
class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject * parent = 0);

    bool setSettings(QString url ="", int id=-1, int ripSecconds =5, QString savePath="." );

    QString url() { return _url; }

    bool start();

public slots:
    void onCameraConnected();
    void onSetuped(int);
    void onPlayed(int);

    void onMotionDetect(int count);

    void ripStart();
    void ripStop();

private:
    int _id;
    QString _url;   
    RTSP * _rtsp;
    H264_Decoder * _decoder;
    MotionDetect * _motionDetect;

    int _ripSecconds;
    QTimer * _ripTimer;
    QThread * _ripThread;
    Ripper * _ripper;

    QString _savePath;
};

typedef QList<Camera*> TCams;

#endif // CAMERA_H
