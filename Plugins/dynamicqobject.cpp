#include "dynamicqobject.h"

DynamicQObject::DynamicQObject( QObject *parent ) :
    QObject(parent)
{
}

bool DynamicQObject::addSlot(QObject *object, const char *signal, const QString &slotName)
{
    if (containsSlot(slotName)) return false;

    if (signal[0] != '2') {
        qWarning() << "Use SIGNAL() macro!";
        return false;
    }

    QByteArray theSignal = QMetaObject::normalizedSignature(&signal[1]);

    int signalId = object->metaObject()->indexOfSignal(theSignal);

    if (signalId < 0) {
        qWarning() << "Signal" << signal << "doesn't exist!";
        return false;
    }

    QVector<int> parameterTypes;
    QMetaMethod signalMethod = object->metaObject()->method(signalId);
    for (int i = 0; i < signalMethod.parameterCount(); ++i) {
        parameterTypes.push_back(signalMethod.parameterType(i));
    }

    int slotIdx = -1;

    for (int i = 0; i < m_slotList.count(); ++i) {
        if (m_slotList[i].isEmpty == true) {
            slotIdx = i;
            break;
        }
    }

    bool addEntry = false;

    if (slotIdx == -1) {
        addEntry = true;
        slotIdx = m_slotList.count();
    }

    if (!QMetaObject::connect(object, signalId, this, slotIdx + metaObject()->methodCount())) {
        qWarning() << "Can't connect" << signal << "signal to virtual slot";
        return false;
    }

    if (addEntry) {
        m_slotList.push_back({false, object, signalId, slotName, parameterTypes});
    } else {
        slot_t &slot = m_slotList[slotIdx];
        slot.isEmpty = false;
        slot.object = object;
        slot.signalIdx = signalId;
        slot.name = slotName;
        slot.parameterTypes = parameterTypes;
    }

    return true;
}

 bool DynamicQObject::removeSlot(const QString &name)
 {
     for (int i = 0; i < m_slotList.count(); ++i) {
         if (m_slotList[i].name==name) {
            m_slotList[i].isEmpty = true;
            return true;
         }
     }

     return false;
 }

int DynamicQObject::qt_metacall(QMetaObject::Call call, int id, void **arguments)
{
    id = QObject::qt_metacall(call, id, arguments);
    if ( id<0 || call!=QMetaObject::InvokeMetaMethod ) { return id; }
    Q_ASSERT(id < m_slotList.size());
    const slot_t &slotInfo = m_slotList[id];
    QVariantList parameters;
    for (int i=0; i<slotInfo.parameterTypes.count(); ++i) {
        void *parameter = arguments[i+1];
        parameters.append(QVariant(QMetaType(slotInfo.parameterTypes[i]), parameter));
    }
    QMetaObject::invokeMethod(m_mapTo, m_catchMethod, Q_ARG(QString, slotInfo.name), Q_ARG(QVariantList, parameters));
    return -1;
}

bool DynamicQObject::addSignal(const QString &name, QObject *object, const char *slot, Qt::ConnectionType connType)
{
    if (slot[0] != '1') {
        qWarning() << "Use SLOT() macro "<< slot;
        return false;
    }

    QByteArray theSlot = QMetaObject::normalizedSignature(&slot[1]);

    int slotIdx = object->metaObject()-> indexOfSlot(theSlot);

    if (slotIdx < 0) {
        qWarning() << "Slot didn't exist "<< theSlot;
        return false;
    }

    QVector<int> parameterTypes;
    QMetaMethod slotMethod = object->metaObject()->method(slotIdx);

    for (int i = 0; i < slotMethod.parameterCount(); ++i) {
        parameterTypes.push_back(slotMethod.parameterType(i));
    }

    int signalIdx = -1;
    for (int i = 0; i < m_slotList.count(); ++i) {
        if (m_slotList[i].isEmpty == true) {
            signalIdx = i;
            break;
        }
    }

    bool addEntry = false;

    if (signalIdx == -1) {
        addEntry = true;
        signalIdx = m_signalList.count();
    }

    if (!QMetaObject::connect(this, signalIdx + metaObject()->methodCount(), object, slotIdx, connType)) {
        qWarning() << "Can't connect virtual signal" << name << "to slot" << theSlot;
        return false;
    }

    if (addEntry) {
        m_signalList.append({false, object, slotIdx, name, parameterTypes, slotMethod.returnType()});
    } else {
        signal_t &signal = m_signalList[signalIdx];
        signal.isEmpty = false;
        signal.reciever = object;
        signal.slotIdx = slotIdx;
        signal.name = name;
        signal.parameterTypes = parameterTypes;
        signal.retType = slotMethod.returnType();
    }

    m_signalHash.insert(name, signalIdx);
    return true;
}

bool DynamicQObject::removeSignal(const QString &name)
{
    if (!m_signalHash.contains(name)) return false;
    int idx = m_signalHash.value(name, -1);
    if (idx==-1) return false;
    m_signalList[idx].isEmpty = true;
    m_signalHash.remove(name);
    return true;
}

QVariantList DynamicQObject::activate(const QString &signalName, const QVariantList &args)
{
    QList<int> list(m_signalHash.values(signalName));
    QVariantList rets;

    foreach (int signalIdx, list) {

        if (signalIdx == -1) {
            qWarning() << "Signal" << signalName << "doesn't exist";
            return rets;
        }

        signal_t &signal = m_signalList[signalIdx];

        if (args.count() < signal.parameterTypes.count()) {
               qWarning()<< "Parameters count mismatch:"<< signalName<< "provided:"<< args.count()<< "need >=:"<< signal.parameterTypes.count();
               continue;
        }

        void * m_parameters[11]; // max 10 parameters + ret value

        for (int i = 0; i < 11; ++i) {
            m_parameters[i]=0;
        }

        QVariantList argsCopy = args;
        for (int i = 0; i < signal.parameterTypes.count(); ++i) {
            if (argsCopy[i].canConvert(signal.parameterTypes[i])) {
                argsCopy[i].convert(signal.parameterTypes[i]);
            } else if (signal.parameterTypes[i]!=41) { // Qvariant no need converts
                //qWarning() << "can't cast parameter " << i<<" type "<<signal.parameterTypes[i] <<" " << signalName<< argsCopy[i].toString()<<argsCopy[i].data();
            }
            m_parameters[i + 1]=argsCopy[i].data();
        }


        m_parameters[0] = QMetaType::create(signal.retType);

        QMetaObject::activate(this, metaObject(), signalIdx + metaObject()->methodCount(), m_parameters);

        if ( signal.retType!=43 ) { // void return type
            QVariant v(QMetaType(signal.retType), m_parameters[0]);
            rets.append(v);
        }

    }

    return rets;
}

bool DynamicQObject::containsSlot(const QString &name)
{
    for (int i = 0; i < m_slotList.count(); ++i) {
        slot_t slot = m_slotList[i];
        if (slot.name==name) {
            return slot.isEmpty;
        }
    }

    return false;
}

DynamicQObject::~DynamicQObject()
{

}
