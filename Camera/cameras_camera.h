#ifndef CAMERAS_CAMERA_H
#define CAMERAS_CAMERA_H

#include <QObject>

#include "rtsp/rtsp.h"



using namespace NS_RSTP;

/**
 * @brief Конкретная камера, точнее её один канал/поток не ебу пока что
 */
class Cameras_Camera : public QObject
{
    Q_OBJECT
public:
    explicit Cameras_Camera(QObject *parent = 0);

    enum Status {
        S_NONE=0,
        S_STARTED=2,
        S_CONNECTED=4,
        S_SETUPED=8,
        S_PLAYED=16,
        S_ERROR=32
    };

    bool setSettings(QString url ="", int id=-1, int channel=0, int streamPort=4041);

    QString url() { return _url; }

    int status();

    bool start();
    bool setup();
    bool play();
    bool stop();

    SDP::sMedia * getSDPMedia();
    NS_RSTP::RTSP_Stream * getStreamer();

signals:

public slots:
    void onCameraConnected();
    void onCameraDisconnected();
    void onChannelDisconnected();
    void onSetuped(int);
    void onPlayed(int);

private:
    int _id;
    QString _url;
    int _streamPort;
    int _channel;
    RTSP * _rtsp;
    int _status;
    int _clientsCount;
};

#endif // CAMERAS_CAMERA_H
