#ifndef CAMERAS_H
#define CAMERAS_H

#include <QObject>
#include "camera.h"
#include "interfaces/icameras.h"
#include <QCoreApplication>

/**
* @brief Управляем списком камер
*/
class Cameras: public QObject, public ICameras
{
  Q_OBJECT
public:
  explicit Cameras(QObject *parent = 0);
  virtual ~Cameras();
  ICamera * newCam(int id) override;
  ICamera * getCam(int id) override;

  TCams getCams() override;
  ISDP * getTotalSDP(int trackId) override;

signals:

public slots:

private:
  TCams _cams;

};

#endif // CAMERAS_H
