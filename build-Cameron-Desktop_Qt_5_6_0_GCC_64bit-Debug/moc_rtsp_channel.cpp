/****************************************************************************
** Meta object code from reading C++ file 'rtsp_channel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Cameron/rtsp/rtsp_channel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rtsp_channel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NS_RSTP__RTSP_Channel_t {
    QByteArrayData data[11];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NS_RSTP__RTSP_Channel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NS_RSTP__RTSP_Channel_t qt_meta_stringdata_NS_RSTP__RTSP_Channel = {
    {
QT_MOC_LITERAL(0, 0, 21), // "NS_RSTP::RTSP_Channel"
QT_MOC_LITERAL(1, 22, 9), // "connected"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 13), // "newReadedData"
QT_MOC_LITERAL(4, 47, 4), // "data"
QT_MOC_LITERAL(5, 52, 11), // "onReadyRead"
QT_MOC_LITERAL(6, 64, 5), // "setup"
QT_MOC_LITERAL(7, 70, 4), // "port"
QT_MOC_LITERAL(8, 75, 4), // "play"
QT_MOC_LITERAL(9, 80, 8), // "teardown"
QT_MOC_LITERAL(10, 89, 5) // "alive"

    },
    "NS_RSTP::RTSP_Channel\0connected\0\0"
    "newReadedData\0data\0onReadyRead\0setup\0"
    "port\0play\0teardown\0alive"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NS_RSTP__RTSP_Channel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   53,    2, 0x0a /* Public */,
       6,    1,   54,    2, 0x0a /* Public */,
       8,    0,   57,    2, 0x0a /* Public */,
       9,    0,   58,    2, 0x0a /* Public */,
      10,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NS_RSTP::RTSP_Channel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RTSP_Channel *_t = static_cast<RTSP_Channel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->newReadedData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->onReadyRead(); break;
        case 3: _t->setup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->play(); break;
        case 5: _t->teardown(); break;
        case 6: _t->alive(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RTSP_Channel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RTSP_Channel::connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RTSP_Channel::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RTSP_Channel::newReadedData)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject NS_RSTP::RTSP_Channel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NS_RSTP__RTSP_Channel.data,
      qt_meta_data_NS_RSTP__RTSP_Channel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NS_RSTP::RTSP_Channel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NS_RSTP::RTSP_Channel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NS_RSTP__RTSP_Channel.stringdata0))
        return static_cast<void*>(const_cast< RTSP_Channel*>(this));
    return QObject::qt_metacast(_clname);
}

int NS_RSTP::RTSP_Channel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void NS_RSTP::RTSP_Channel::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void NS_RSTP::RTSP_Channel::newReadedData(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
