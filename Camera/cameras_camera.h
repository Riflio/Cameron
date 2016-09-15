#ifndef CAMERAS_CAMERA_H
#define CAMERAS_CAMERA_H

#include <QObject>

class Cameras_Camera : public QObject
{
    Q_OBJECT
public:
    explicit Cameras_Camera(QObject *parent = 0);

signals:

public slots:
};

#endif // CAMERAS_CAMERA_H