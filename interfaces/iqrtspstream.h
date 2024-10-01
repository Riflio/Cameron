#ifndef IQRTSPSTREAM_H
#define IQRTSPSTREAM_H

#include <QObject>
#include "interfaces/irtspstream.h"

namespace NS_RSTP {

class IQRTSPStream: public QObject, public IRTSPStream
{
  Q_OBJECT
public:
  explicit IQRTSPStream(QObject *parent =nullptr): QObject(parent) {}

signals:
  void newPacketAvaliable(QSharedPointer<IRTPPacket> packet) override final;

};

}

#endif // IQRTSPSTREAM_H
