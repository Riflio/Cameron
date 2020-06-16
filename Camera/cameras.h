#ifndef CAMERAS_H
#define CAMERAS_H

#include <QObject>
#include "cameras_camera.h"
#include "Interfaces/icameras.h"
#include <QCoreApplication>


/**
 * @brief Управляем списком камер
 */
class Cameras : public QObject, public ICameras
{
    Q_OBJECT
public:
    explicit Cameras(QObject *parent = 0);
    virtual ~Cameras();
    ICameras_Camera * newCam(int id);
    ICameras_Camera * getCam(int id);

    TCams getCams();
    ISDP * getTotalSDP();

signals:

public slots:

private:
    TCams _cams;

};

#endif // CAMERAS_H
