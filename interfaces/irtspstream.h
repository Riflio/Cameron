#ifndef IRTSPSTREAM_H
#define IRTSPSTREAM_H

#include <QObject>
#include <QSharedPointer>
#include "interfaces/irtppacket.h"

namespace NS_RSTP {

class IRTSPStream
{
public:
  virtual ~IRTSPStream() {};

signals:
  virtual void newPacketAvaliable(QSharedPointer<IRTPPacket> packet) =0;

};

}

#endif // IRTSPSTREAM_H
