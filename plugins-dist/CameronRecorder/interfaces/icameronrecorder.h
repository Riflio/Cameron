#ifndef ICAMERONRECORDER_H
#define ICAMERONRECORDER_H

#include <QObject>

#include "interfaces/irecorder.h"

class ICameronRecorder
{
public:
  virtual ~ICameronRecorder() {}
  virtual IRecorder *getRecorder(int id) const =0;
};

#endif // ICAMERONRECORDER_H
