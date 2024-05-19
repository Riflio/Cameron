#ifndef ICAMERAS_H
#define ICAMERAS_H

#include "icamera.h"
#include <QHash>

typedef QHash<int, ICamera*> TCams;

class ICameras
{
public:
  virtual ~ICameras() {}
  virtual ICamera *newCam(int id) =0;
  virtual ICamera *getCam(int id) =0;
  virtual TCams getCams() =0;
  virtual ISDP *getTotalSDP(int trackId) =0;
};

#endif // ICAMERAS_H
