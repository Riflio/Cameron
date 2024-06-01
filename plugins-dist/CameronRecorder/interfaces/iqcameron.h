#ifndef IQCAMERON_H
#define IQCAMERON_H

#include <QObject>
#include "interfaces/icameron.h"

class IQCameron: public QObject, public ICameron
{
  Q_OBJECT
public:
  IQCameron(QObject *parent): QObject(parent) {}

signals:
  void started() override;

};

#endif // IQCAMERON_H
