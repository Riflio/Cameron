#include "rtp.h"
#include <QDebug>
namespace NS_RSTP {

RTP::RTP(QObject * parent):
    QObject(parent), MultiAccessBuffer(5000), _mutex()
{

}

/**
 * @brief Нам прислали новый rtp пакет, добавляем в очередь
 * @param packet
 * @return
 */
bool RTP::newPacket(QByteArray packet)
{
    QMutexLocker locker(&_mutex);

    if (packet.isEmpty()) return false;

    return put(packet);

}


/**
 * @brief берём из очереди следующий пакет и засовываем в _curPacket для дальнейших манипуляций
 * @return
 */
bool RTP::getPacket(long long & offset, QByteArray & packet)
{
    if ( !get(offset, _curPacket) ) return false;

    packet = _curPacket;

    return true;
}


int RTP::getPayloadStart()
{
    return RTP_HEADER_SIZE + getCC() * 4;
}

int RTP::getPayloadLength()
{
    return _curPacket.length() - getPayloadStart();
}

/**
 * @brief Количество CSRC идентификаторов
 * @return
 */
BYTE RTP::getCC()
{
    BYTE c = (BYTE)((_curPacket[0]) & 0x0f);
    return  c;
}

bool  RTP::hasPadding()
{
    return ((_curPacket[0] & 0x20) >> 5) == 1;
}


unsigned int RTP::getTimeStamp()
{
    QByteArray ba = _curPacket;
    int idx = 4; //-- с какой позиции начиная

    //-- собираем 4 байта в int
    unsigned int i = (((ba[idx] << 24) & 0xFF000000L)
        | ((ba[idx + 1] << 16) & 0xFF0000L)
        | (ba[idx + 2] << 8 & 0xFF00L)
        | (ba[idx + 3] & 0xFFL));

    return i;
}


unsigned short RTP::getSequence()
{
    unsigned short s;

    QByteArray ba = _curPacket;
    int idx = 2; //-- с какой позиции начиная

    //-- собираем 2 байта в unsigned short
    s = (((ba[idx] << 8) & 0xFF00) | (ba[idx+1] & 0xFF));

    return s;
}


}
