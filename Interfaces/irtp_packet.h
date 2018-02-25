#ifndef IRTP_PACKET_H
#define IRTP_PACKET_H

#include <QByteArray>

typedef char BYTE;

class IRTP_Packet
{
public:
    virtual ~IRTP_Packet() {}

    virtual int getPayloadStart()=0;
    virtual int getPayloadLength()=0;

    virtual bool hasPadding()=0;

    virtual BYTE  getCC()=0;
    virtual unsigned int getTimeStamp()=0;
    virtual unsigned short getSequence()=0;

    virtual QByteArray &data()=0;
};

#endif // IRTP_PACKET_H
