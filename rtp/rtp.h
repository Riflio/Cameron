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
    virtual ~RTP();

    bool newPacket(QByteArray packet) override;
    IRTP_Packet * getPacket(int & offset) const override;
    const QByteArray * getPacketData(int &offset) const override;
    int bufOffset() const override;

private:   
    QMutex _mutex;    
};

}

#endif // RTP_H
