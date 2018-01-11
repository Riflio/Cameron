#ifndef IRTP_H
#define IRTP_H

#include <QByteArray>

#include "irtp_packet.h"

namespace NS_RSTP {

class IRTP
{
public:
    virtual bool newPacket(QByteArray packet)=0;
    virtual bool getPacket(long long & offset, IRTP_Packet *& packet)=0;
    virtual bool getPacketData(long long & offset, QByteArray & packetData)=0;
    virtual long long int bufOffset()=0;
};

}
#endif // IRTP_H
