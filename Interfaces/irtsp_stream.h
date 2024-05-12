#ifndef IRTSP_STREAM_H
#define IRTSP_STREAM_H

#include <QSharedPointer>
#include "Interfaces/irtp_packet.h"

namespace NS_RSTP {

class IRTSP_Stream
{
public:
  virtual ~IRTSP_Stream() {};
  virtual void newPacketAvaliable(QSharedPointer<IRTP_Packet> packet) =0;
};

}

#endif // IRTSP_STREAM_H
