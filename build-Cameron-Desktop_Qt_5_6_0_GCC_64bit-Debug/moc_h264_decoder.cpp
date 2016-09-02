/****************************************************************************
** Meta object code from reading C++ file 'h264_decoder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Cameron/h264/h264_decoder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'h264_decoder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NS_H264__H264_Decoder_t {
    QByteArrayData data[8];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NS_H264__H264_Decoder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NS_H264__H264_Decoder_t qt_meta_stringdata_NS_H264__H264_Decoder = {
    {
QT_MOC_LITERAL(0, 0, 21), // "NS_H264::H264_Decoder"
QT_MOC_LITERAL(1, 22, 19), // "newDecodedFrameData"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 23), // "QSharedPointer<uint8_t>"
QT_MOC_LITERAL(4, 67, 4), // "data"
QT_MOC_LITERAL(5, 72, 5), // "width"
QT_MOC_LITERAL(6, 78, 6), // "height"
QT_MOC_LITERAL(7, 85, 7) // "process"

    },
    "NS_H264::H264_Decoder\0newDecodedFrameData\0"
    "\0QSharedPointer<uint8_t>\0data\0width\0"
    "height\0process"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NS_H264__H264_Decoder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int,    4,    5,    6,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void NS_H264::H264_Decoder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        H264_Decoder *_t = static_cast<H264_Decoder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newDecodedFrameData((*reinterpret_cast< QSharedPointer<uint8_t>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->process(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (H264_Decoder::*_t)(QSharedPointer<uint8_t> , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&H264_Decoder::newDecodedFrameData)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject NS_H264::H264_Decoder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NS_H264__H264_Decoder.data,
      qt_meta_data_NS_H264__H264_Decoder,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NS_H264::H264_Decoder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NS_H264::H264_Decoder::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NS_H264__H264_Decoder.stringdata0))
        return static_cast<void*>(const_cast< H264_Decoder*>(this));
    return QObject::qt_metacast(_clname);
}

int NS_H264::H264_Decoder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void NS_H264::H264_Decoder::newDecodedFrameData(QSharedPointer<uint8_t> _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
