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

    void addAction(QString, QObject *, const char *);
    void addAction(QString, QVariant, QString);

public:
    DynamicQObject *  eWrapper;
    QVariant ARG(void * p) { return qVariantFromValue((void *)p); }

};


#endif // EVENTER_H
