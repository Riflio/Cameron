#ifndef IRTP_H
#define IRTP_H

#include <QByteArray>

typedef char BYTE;

namespace NS_RSTP {

class IRTP
{
public:
    virtual bool newPacket(QByteArray packet)=0;

    virtual bool getPacket(long long & offset, QByteArray & packet)=0;

    virtual int getPayloadStart()=0;
    virtual int getPayloadLength()=0;

    virtual bool hasPadding()=0;

    virtual BYTE  getCC()=0;
    virtual unsigned int getTimeStamp()=0;
    virtual unsigned short getSequence()=0;
};

}
#endif // IRTP_H
