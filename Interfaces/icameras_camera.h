#ifndef ICAMERAS_CAMERA_H
#define ICAMERAS_CAMERA_H

#include <QString>
#include "isdp.h"
#include "irtsp_stream.h"

class ICameras_Camera
{
public:

    enum Status {
        S_NONE=0,
        S_STARTED=2,
        S_CONNECTED=4,
        S_SETUPED=8,
        S_PLAYED=16,
        S_ERROR=32
    };

    virtual bool setSettings(QString url ="", int id =-1, int channel =0, int streamPort=4041) =0;
    virtual int id() =0;
    virtual QString url() =0;
    virtual int status() =0;
    virtual bool start() =0;
    virtual bool setup() =0;
    virtual bool play() =0;
    virtual bool stop() =0;
    virtual bool go() =0;
    virtual ISDP::sMedia * getSDPMedia() =0;
    virtual NS_RSTP::IRTSP_Stream * getStreamer() =0;

};

#endif // ICAMERAS_CAMERA_H
