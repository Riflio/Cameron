#ifndef IRECORDER_H
#define IRECORDER_H

#include "interfaces/recfileinfo.h"

class IRecorder
{
public:
  virtual ~IRecorder() {}

  virtual quint32 id() const =0;
  virtual QMap<quint64, TRecFileInfo> filesInfo() const =0;
  virtual TRecFileInfo fileInfo() const =0;

  virtual bool checkNeedNewFile() const =0;
  virtual bool makeNewRecFile() =0;
  virtual bool makeNewRecFileIfNeed() =0;

  virtual bool startRec() =0;
  virtual bool stopRec() =0;

public slots:


};

#endif // IRECORDER_H
