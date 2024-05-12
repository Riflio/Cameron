#ifndef ISERVER_H
#define ISERVER_H

#include "icameras.h"

class IServer
{
public:
  virtual ~IServer() {}
  virtual void setCams(ICameras *cameras) =0;
  virtual ICameras *getCams() =0;

};

#endif // ISERVER_H
