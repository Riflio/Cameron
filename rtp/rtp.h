#ifndef RTP_H
#define RTP_H

#include <QObject>
#include <QByteArray>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include "../Interfaces/irtp.h"
#include "Assets/multiaccessbuffer.h"

/**
 * @brief Базовый класс для принятого голого RTP пакета
 */

namespace NS_RSTP {

class RTP: public virtual IRTP, private MultiAccessBuffer<QByteArray>
{
public:
    explicit RTP();

    const int RTP_HEADER_SIZE = 12;

    bool newPacket(QByteArray packet);

    bool getPacket(long long & offset, QByteArray & packet);

    int getPayloadStart();
    int getPayloadLength();

    bool hasPadding();

    BYTE  getCC();
    unsigned int getTimeStamp();
    unsigned short getSequence();

private:   
    QMutex _mutex;
    QByteArray _curPacket; //-- текущий пакет для обработки
};

}

#endif // RTP_H
