#ifndef RTP_H
#define RTP_H

#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>

#include "Interfaces/irtp.h"
#include "Assets/circleBufferWriter.h"

#include "rtp_packet.h"

/**
* @brief Базовый класс для принятых голых RTP пакетов
*/
namespace NS_RSTP {


class RTP: public virtual IRTP
{
public:
    explicit RTP();
    virtual ~RTP();

    bool newPacket(const QByteArray &packetData) override;
    TIRTPPacketsBuffer * rtpPacketsBuffer() const override;

private:   
    QMutex _mutex;

    typedef CircleBufferWriter<RTP_packet> TRTPPacketsBuffer;
    TRTPPacketsBuffer * _rtpPacketsBuffer;

};

}

#endif // RTP_H
