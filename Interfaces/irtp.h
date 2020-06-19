#ifndef IRTP_H
#define IRTP_H

#include <QByteArray>

#include "irtp_packet.h"

namespace NS_RSTP {

class IRTP
{
public:
    virtual bool newPacket(QByteArray packet) =0;
    virtual IRTP_Packet * getPacket(int & offset) const =0;
    virtual const QByteArray * getPacketData(int & offset) const =0;
    virtual int bufOffset() const =0;

};

}
#endif // IRTP_H
