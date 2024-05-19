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
  virtual const QMap<uint16_t, QByteArray> &metaInfo() const =0;
  virtual bool setMetaInfo(const QMap<uint16_t, QByteArray> &newMetaInfo) =0;

signals:
  virtual void newPacketAvaliable(QSharedPointer<IRTPPacket> packet) =0;
  virtual void metaInfoChanged() =0;
};

}

#endif // IRTSPSTREAM_H
