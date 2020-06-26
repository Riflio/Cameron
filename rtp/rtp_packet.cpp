#include "rtp_packet.h"

RTP_packet::RTP_packet()
{

}

QByteArray RTP_packet::data() const
{
    return _data;
}

void RTP_packet::setData(const QByteArray &data)
{
    _data = data;
}

RTP_packet &RTP_packet::operator=(const QByteArray &data)
{
    _data = data;
    return *this;
}

RTP_packet &RTP_packet::operator=(const RTP_packet &packet)
{
    _data = packet._data;
    return *this;
}

int RTP_packet::getPayloadStart() const
{
    return RTP_HEADER_SIZE + getCC() * 4;
}

int RTP_packet::getPayloadLength() const
{
    return _data.length() - getPayloadStart();
}

/**
* @brief Количество CSRC идентификаторов
* @return
*/
BYTE RTP_packet::getCC() const
{
    BYTE c = (BYTE)((_data[0]) & 0x0f);
    return  c;
}

bool RTP_packet::hasPadding() const
{
    return ((_data[0] & 0x20) >> 5) == 1;
}

unsigned int RTP_packet::getTimeStamp() const
{    
    int idx = 4; //-- С какой позиции начиная

    //-- Собираем 4 байта в int
    unsigned int i = (((_data[idx] << 24) & 0xFF000000L)
        | ((_data[idx + 1] << 16) & 0xFF0000L)
        | (_data[idx + 2] << 8 & 0xFF00L)
        | (_data[idx + 3] & 0xFFL));

    return i;
}


unsigned short RTP_packet::getSequence() const
{
    unsigned short s;

    int idx = 2; //-- С какой позиции начиная

    //-- Собираем 2 байта в unsigned short
    s = (((_data[idx] << 8) & 0xFF00) | (_data[idx+1] & 0xFF));

    return s;
}
