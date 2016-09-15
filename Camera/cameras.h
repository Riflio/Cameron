#ifndef CAMERAS_H
#define CAMERAS_H

#include <QObject>
#include "cameras_camera.h"

typedef QHash<int, Cameras_Camera*> TCams;

class Cameras : public QObject
{
    Q_OBJECT
public:
    explicit Cameras(QObject *parent = 0);

    Cameras_Camera * newCam(int id);

    TCams getCams();

    SDP * getTotalSDP();

signals:

public slots:

private:
    TCams _cams;
};

#endif // CAMERAS_H
