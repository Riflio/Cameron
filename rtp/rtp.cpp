#include "rtp.h"
#include <QDebug>
namespace NS_RSTP {

RTP::RTP()
    : MultiAccessBuffer(500), _mutex()
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

    if ( packet.isEmpty() ) return false;

    return put(packet);

}


/**
* @brief Берём из очереди данные по заданному смещению и отдаём
* @return
*/
bool RTP::getPacketData(long long & offset, QByteArray & packet)
{
    if ( !get(offset, packet) ) return false;

    return true;
}

long long int RTP::bufOffset()
{
    return offset(0);
}

/**
* @brief Берём из очереди данные по заданному смещению и отдаём пакет
* @param offset
* @param packet
* @return
*/
bool RTP::getPacket(long long & offset, IRTP_Packet *& packet)
{    

    QByteArray data;
    if ( !get(offset, data) ) { packet = nullptr; return false; }

    packet = new RTP_packet();
    packet->setData(data);

    return true;
}



}
