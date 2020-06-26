#ifndef RTP_PACKET_H
#define RTP_PACKET_H

#include "Interfaces/irtp_packet.h"
#include <QByteArray>


class RTP_packet: public IRTP_Packet
{
public:
    RTP_packet();
    virtual ~RTP_packet() {}

    int getPayloadStart() const;
    int getPayloadLength() const;

    bool hasPadding() const;

    BYTE getCC() const;
    unsigned int getTimeStamp() const;
    unsigned short getSequence() const;
    const int RTP_HEADER_SIZE = 12;

    QByteArray data() const;
    void setData(const QByteArray &data);

    RTP_packet& operator=(const QByteArray & data);
    RTP_packet& operator=(const RTP_packet & packet);

private:
    QByteArray _data;

};

#endif // RTP_PACKET_H
