#ifndef RTP_H
#define RTP_H

#include <QObject>
#include <QByteArray>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include "../Interfaces/irtp.h"
#include "Assets/multiaccessbuffer.h"

#include "rtp_packet.h"

/**
 * @brief Базовый класс для принятого голого RTP пакета
 */

namespace NS_RSTP {


class RTP: public virtual IRTP, private MultiAccessBuffer<QByteArray>
{
public:
    explicit RTP();

    bool newPacket(QByteArray packet);
    bool getPacket(long long & offset, IRTP_Packet *& packet);
    bool getPacketData(long long & offset, QByteArray & packet);
    long long int bufOffset();

private:   
    QMutex _mutex;    
};

}

#endif // RTP_H
