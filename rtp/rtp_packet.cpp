#include "rtp_packet.h"

RTP_packet::RTP_packet(const QByteArray & packetData ):
    _packet(packetData)
{

}

QByteArray RTP_packet::data()
{
    return _packet;
}

int RTP_packet::getPayloadStart()
{
    return RTP_HEADER_SIZE + getCC() * 4;
}

int RTP_packet::getPayloadLength()
{
    return _packet.length() - getPayloadStart();
}

/**
 * @brief Количество CSRC идентификаторов
 * @return
 */
BYTE RTP_packet::getCC()
{
    BYTE c = (BYTE)((_packet[0]) & 0x0f);
    return  c;
}

bool RTP_packet::hasPadding()
{
    return ((_packet[0] & 0x20) >> 5) == 1;
}


unsigned int RTP_packet::getTimeStamp()
{
    QByteArray ba = _packet;
    int idx = 4; //-- с какой позиции начиная

    //-- собираем 4 байта в int
    unsigned int i = (((ba[idx] << 24) & 0xFF000000L)
        | ((ba[idx + 1] << 16) & 0xFF0000L)
        | (ba[idx + 2] << 8 & 0xFF00L)
        | (ba[idx + 3] & 0xFFL));

    return i;
}


unsigned short RTP_packet::getSequence()
{
    unsigned short s;

    QByteArray ba = _packet;
    int idx = 2; //-- с какой позиции начиная

    //-- собираем 2 байта в unsigned short
    s = (((ba[idx] << 8) & 0xFF00) | (ba[idx+1] & 0xFF));

    return s;
}
