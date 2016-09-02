/****************************************************************************
** Meta object code from reading C++ file 'h264_stream_rtp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Cameron/h264/h264_stream_rtp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'h264_stream_rtp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NS_H264__H264_Stream_RTP_t {
    QByteArrayData data[5];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NS_H264__H264_Stream_RTP_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NS_H264__H264_Stream_RTP_t qt_meta_stringdata_NS_H264__H264_Stream_RTP = {
    {
QT_MOC_LITERAL(0, 0, 24), // "NS_H264::H264_Stream_RTP"
QT_MOC_LITERAL(1, 25, 8), // "finished"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 5), // "parse"
QT_MOC_LITERAL(4, 41, 7) // "process"

    },
    "NS_H264::H264_Stream_RTP\0finished\0\0"
    "parse\0process"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NS_H264__H264_Stream_RTP[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Bool,

       0        // eod
};

void NS_H264::H264_Stream_RTP::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        H264_Stream_RTP *_t = static_cast<H264_Stream_RTP *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: { bool _r = _t->parse();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->process();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (H264_Stream_RTP::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&H264_Stream_RTP::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject NS_H264::H264_Stream_RTP::staticMetaObject = {
    { &NS_RSTP::RTSP_Stream::staticMetaObject, qt_meta_stringdata_NS_H264__H264_Stream_RTP.data,
      qt_meta_data_NS_H264__H264_Stream_RTP,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NS_H264::H264_Stream_RTP::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NS_H264::H264_Stream_RTP::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NS_H264__H264_Stream_RTP.stringdata0))
        return static_cast<void*>(const_cast< H264_Stream_RTP*>(this));
    return NS_RSTP::RTSP_Stream::qt_metacast(_clname);
}

int NS_H264::H264_Stream_RTP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NS_RSTP::RTSP_Stream::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NS_H264::H264_Stream_RTP::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
