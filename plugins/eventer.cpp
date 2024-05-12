#include "eventer.h"

#include <QStringList>
#include <QMetaObject>

Eventer::Eventer(QObject *parent) :
    QObject(parent)
{
    eWrapper = new DynamicQObject(this);
}

QVariantList Eventer::doAction(QString signalName, QVariantList args)
{
    return eWrapper->activate(signalName, args);
}

QVariantList Eventer::doAction(QString signalName, QVariant args)
{
    return eWrapper->activate(signalName, args.toList());
}



void Eventer::addAction(QString name, QObject *object, const char *slot, Qt::ConnectionType connType)
{
    eWrapper->addSignal(name, object, slot, connType);
}

void Eventer::addAction(QString name, QVariant object, QString slot, Qt::ConnectionType connType)
{
    eWrapper->addSignal(name, qvariant_cast<QObject *>(object), slot.toLocal8Bit().data(), connType);
}
