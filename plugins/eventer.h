#ifndef EVENTER_H
#define EVENTER_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QVariant>
#include <QVariantList>
#include "dynamicqobject.h"
#include "usingleton.h"


class Eventer : public QObject
{
    Q_OBJECT
public:
    explicit Eventer(QObject *parent = 0);

public slots:
    QVariantList doAction(QString, QVariantList);
    QVariantList doAction(QString, QVariant);

    void addAction(QString, QObject *, const char *, Qt::ConnectionType connType = Qt::AutoConnection);
    void addAction(QString, QVariant, QString,  Qt::ConnectionType connType = Qt::AutoConnection);

public:
    DynamicQObject *  eWrapper;
    QVariant ARG(void * p) { return QVariant::fromValue((void *)p); }

};


#endif // EVENTER_H
