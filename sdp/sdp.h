#ifndef SDP_H
#define SDP_H

#include <QObject>
#include <QMap>
#include <QMultiMap>
#include <QHostAddress>
#include "../Interfaces/isdp.h"
/**
 * @brief Парсим/формируем SDP формат
 */
class SDP : public QObject, public ISDP
{
    Q_OBJECT
public:
    explicit SDP(QObject *parent = 0);
    virtual ~SDP();

    bool parse(QByteArray data);
    bool make(QByteArray & data);



    sOrigin origin;
    QList<sMedia*> medias;

signals:

public slots:

private:
    QByteArray _data;
};

#endif // SDP_H
