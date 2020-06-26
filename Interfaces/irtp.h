#ifndef IRTP_H
#define IRTP_H


#include "Assets/circleBufferWriter.h"
#include "irtp_packet.h"


namespace NS_RSTP {

class IRTP
{
public:

    static const int BUFFER_SIZE = 200;
    typedef IBuffer<IRTP_Packet> TIRTPPacketsBuffer;

    virtual bool newPacket(const QByteArray &packetData) =0;
    virtual TIRTPPacketsBuffer * rtpPacketsBuffer() const =0;
};

}
#endif // IRTP_H
