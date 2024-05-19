#ifndef IQCAMERA_H
#define IQCAMERA_H

#include <QObject>
#include "interfaces/icamera.h"

class IQCamera: public QObject, public ICamera
{
  Q_OBJECT
public:
  explicit IQCamera(QObject *parent): QObject(parent) {}

signals:
  void errored() override final;
  void connected() override final;
  void disconnected() override final;
  void setuped() override final;
  void played() override final;
  void teardowned() override final;
};



#endif // IQCAMERA_H
