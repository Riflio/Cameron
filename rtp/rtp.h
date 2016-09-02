#ifndef RTP_H
#define RTP_H

#include <QObject>
#include <QByteArray>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
/**
 * @brief Базовый класс для принятого голого RTP пакета
 */

namespace NS_RSTP {


typedef char BYTE;

class RTP: public QObject
{
    Q_OBJECT
public:
    explicit RTP(QObject * parent);

    const int RTP_HEADER_SIZE = 12;

    bool newPacket(QByteArray packet);

    bool getPacket();

    int getPayloadStart();
    int getPayloadLength();

    bool hasPadding();

    BYTE  getCC();
    unsigned int getTimeStamp();
    unsigned short getSequence();


protected:
    QByteArray _curPacket; //-- текущий пакет для обработки

private:
    QQueue<QByteArray> _packets;
    QMutex _mutex;
};

}

#endif // RTP_H
