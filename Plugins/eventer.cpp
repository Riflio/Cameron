#include "eventer.h"

#include <QStringList>
#include <QMetaObject>

Eventer::Eventer(QObject *parent) :
    QObject(parent)
{
    eWrapper = new DynamicQObject(this);
}

QVariantList Eventer::doAction(QString signalName, QVariantList args) {
    return eWrapper->activate(signalName, args);
}

QVariantList Eventer::doAction(QString signalName, QVariant args) {
    return eWrapper->activate(signalName, args.toList());
}



void Eventer::addAction(QString name, QObject *object, const char *slot) {
   /* QObject * obj=object;
    const QMetaObject* metaObject = obj->metaObject();
    QStringList methods;
    for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i) {
        if (metaObject->method(i).methodType() == QMetaMethod::Slot) {
            qDebug()<<"SSSSSSSSSSSSSSS"<< QString::fromLatin1(metaObject->method(i).methodSignature());
        }
    }*/

    eWrapper->addSignal(name, object, slot);
}

void Eventer::addAction(QString name, QVariant object, QString slot) {
    eWrapper->addSignal(name, qvariant_cast<QObject *>(object), slot.toLocal8Bit().data());
}
