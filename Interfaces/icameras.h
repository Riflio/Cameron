#ifndef ICAMERAS_H
#define ICAMERAS_H

#include "icameras_camera.h"
#include <QHash>

typedef QHash<int, ICameras_Camera*> TCams;

class ICameras
{
public:
    virtual ICameras_Camera * newCam(int id)=0;
    virtual ICameras_Camera * getCam(int id)=0;
    virtual TCams getCams()=0;
    virtual ISDP * getTotalSDP()=0;
};

#endif // ICAMERAS_H
