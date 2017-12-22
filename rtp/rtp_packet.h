#ifndef RTP_PACKET_H
#define RTP_PACKET_H

#include "Interfaces/irtp_packet.h"
#include <QByteArray>


class RTP_packet: public IRTP_Packet
{
public:
    RTP_packet(const QByteArray & packetData );


    int getPayloadStart();
    int getPayloadLength();

    bool hasPadding();

    BYTE  getCC();
    unsigned int getTimeStamp();
    unsigned short getSequence();
    const int RTP_HEADER_SIZE = 12;

    QByteArray data();

private:
    QByteArray _packet;
};

#endif // RTP_PACKET_H
