/****************************************************************************
** Meta object code from reading C++ file 'rtsp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Cameron/rtsp/rtsp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rtsp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NS_RSTP__RTSP_t {
    QByteArrayData data[19];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NS_RSTP__RTSP_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NS_RSTP__RTSP_t qt_meta_stringdata_NS_RSTP__RTSP = {
    {
QT_MOC_LITERAL(0, 0, 13), // "NS_RSTP::RTSP"
QT_MOC_LITERAL(1, 14, 9), // "connected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "setuped"
QT_MOC_LITERAL(4, 33, 7), // "channel"
QT_MOC_LITERAL(5, 41, 6), // "played"
QT_MOC_LITERAL(6, 48, 10), // "teardowned"
QT_MOC_LITERAL(7, 59, 13), // "cameraConnect"
QT_MOC_LITERAL(8, 73, 12), // "QHostAddress"
QT_MOC_LITERAL(9, 86, 7), // "address"
QT_MOC_LITERAL(10, 94, 4), // "port"
QT_MOC_LITERAL(11, 99, 6), // "params"
QT_MOC_LITERAL(12, 106, 3), // "url"
QT_MOC_LITERAL(13, 110, 21), // "onSckConnectReadyRead"
QT_MOC_LITERAL(14, 132, 21), // "onSckConnectConnected"
QT_MOC_LITERAL(15, 154, 24), // "onSckConnectDisconnected"
QT_MOC_LITERAL(16, 179, 17), // "onSckConnectError"
QT_MOC_LITERAL(17, 197, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(18, 226, 24) // "onSckConnectBytesWritten"

    },
    "NS_RSTP::RTSP\0connected\0\0setuped\0"
    "channel\0played\0teardowned\0cameraConnect\0"
    "QHostAddress\0address\0port\0params\0url\0"
    "onSckConnectReadyRead\0onSckConnectConnected\0"
    "onSckConnectDisconnected\0onSckConnectError\0"
    "QAbstractSocket::SocketError\0"
    "onSckConnectBytesWritten"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NS_RSTP__RTSP[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    1,   70,    2, 0x06 /* Public */,
       5,    1,   73,    2, 0x06 /* Public */,
       6,    1,   76,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    3,   79,    2, 0x0a /* Public */,
       7,    1,   86,    2, 0x0a /* Public */,
      13,    0,   89,    2, 0x0a /* Public */,
      14,    0,   90,    2, 0x0a /* Public */,
      15,    0,   91,    2, 0x0a /* Public */,
      16,    1,   92,    2, 0x0a /* Public */,
      18,    1,   95,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int, QMetaType::QString,    9,   10,   11,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,    2,
    QMetaType::Void, QMetaType::LongLong,    2,

       0        // eod
};

void NS_RSTP::RTSP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RTSP *_t = static_cast<RTSP *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->setuped((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->played((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->teardowned((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->cameraConnect((*reinterpret_cast< QHostAddress(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->cameraConnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->onSckConnectReadyRead(); break;
        case 7: _t->onSckConnectConnected(); break;
        case 8: _t->onSckConnectDisconnected(); break;
        case 9: _t->onSckConnectError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 10: _t->onSckConnectBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RTSP::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RTSP::connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RTSP::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RTSP::setuped)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RTSP::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RTSP::played)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RTSP::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RTSP::teardowned)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject NS_RSTP::RTSP::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NS_RSTP__RTSP.data,
      qt_meta_data_NS_RSTP__RTSP,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NS_RSTP::RTSP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NS_RSTP::RTSP::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NS_RSTP__RTSP.stringdata0))
        return static_cast<void*>(const_cast< RTSP*>(this));
    return QObject::qt_metacast(_clname);
}

int NS_RSTP::RTSP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void NS_RSTP::RTSP::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void NS_RSTP::RTSP::setuped(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NS_RSTP::RTSP::played(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NS_RSTP::RTSP::teardowned(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
