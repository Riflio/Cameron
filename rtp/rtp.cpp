#include "rtp.h"
#include <QDebug>
namespace NS_RSTP {

RTP::RTP()
    : MultiAccessBuffer(500), _mutex()
{

}

RTP::~RTP()
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
const QByteArray * RTP::getPacketData(int & offset) const
{
    const QByteArray * packet = get(offset);
    if ( packet==nullptr ) { return nullptr; }
    return packet;
}

int RTP::bufOffset() const
{
    return cur();
}

/**
* @brief Берём из очереди данные по заданному смещению и отдаём пакет
* @param offset
* @param packet
* @return
*/
IRTP_Packet * RTP::getPacket(int &offset) const
{    

    const QByteArray *data = get(offset);
    if ( data==nullptr ) { return nullptr; }

    IRTP_Packet * packet = new RTP_packet();
    packet->setData(*data);

    return packet;
}



}
