#ifndef DYNAMICQOBJECT_H
#define DYNAMICQOBJECT_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <QVariant>
#include <QMetaMethod>

#include <QDebug>

//-- Гдеж ты раньше был... http://habrahabr.ru/post/215181/

class DynamicQObject : public QObject
{
public:
    DynamicQObject( QObject *parent = 0);
    ~DynamicQObject();

    bool addSlot(QObject *object, const char *signal, const QString &slotName);
    bool removeSlot(const QString &name);

    bool addSignal(const QString &name, QObject *object, const char *slot);
    bool removeSignal(const QString &name);
    QVariantList activate(const QString &signalName, const QVariantList &args);

    int qt_metacall(QMetaObject::Call call, int id, void **arguments);
private:
    // virtual slots
    bool containsSlot(const QString &name);
    QObject *m_mapTo;
    const char *m_catchMethod;
    typedef struct {
        bool isEmpty; // true after removeSlot()
        QObject *object;
        int signalIdx;
        QString name; // virtual slot name
        QVector<int> parameterTypes;
    } slot_t;
    QVector<slot_t> m_slotList;

    // virtual signals
    typedef struct {
        bool isEmpty; // // true after removeSignal()
        QObject *reciever;
        int slotIdx;
        QString name;
        QVector<int> parameterTypes;
        int retType;
    } signal_t;
    QVector<signal_t> m_signalList;
    QMultiHash<QString, int> m_signalHash;
    void * m_parameters[11]; // max 10 parameters + ret value

};

#endif // DYNAMICQOBJECT_H
