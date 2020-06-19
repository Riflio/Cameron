#ifndef IRTP_PACKET_H
#define IRTP_PACKET_H

#include <QByteArray>

typedef char BYTE;

class IRTP_Packet
{
public:
    virtual ~IRTP_Packet() {}

    virtual int getPayloadStart() const =0;
    virtual int getPayloadLength() const =0;

    virtual bool hasPadding() const =0;

    virtual BYTE getCC() const =0;
    virtual unsigned int getTimeStamp() const =0;
    virtual unsigned short getSequence() const =0;

    virtual QByteArray data() const =0;
    virtual void setData(const QByteArray &data) =0;
};

#endif // IRTP_PACKET_H
