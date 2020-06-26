#include "rtp.h"
#include <QDebug>
#include <QThread>

namespace NS_RSTP {

RTP::RTP()
    : _mutex()
{
    _rtpPacketsBuffer = new TRTPPacketsBuffer(BUFFER_SIZE);

}

RTP::~RTP()
{
    delete _rtpPacketsBuffer;
}

IRTP::TIRTPPacketsBuffer *RTP::rtpPacketsBuffer() const
{
    return reinterpret_cast<IRTP::TIRTPPacketsBuffer*>(_rtpPacketsBuffer);
}

/**
* @brief Нам прислали новый rtp пакет, добавляем в очередь
* @param packet
* @return
*/
bool RTP::newPacket(const QByteArray &packetData)
{
    QMutexLocker locker(&_mutex);
    if ( packetData.isEmpty() ) return false;

    return _rtpPacketsBuffer->put(packetData);
}

}
