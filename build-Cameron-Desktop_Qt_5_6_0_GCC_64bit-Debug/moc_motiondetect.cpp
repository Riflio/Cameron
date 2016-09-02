/****************************************************************************
** Meta object code from reading C++ file 'motiondetect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Cameron/OpenCV/motiondetect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'motiondetect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MotionDetect_t {
    QByteArrayData data[9];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MotionDetect_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MotionDetect_t qt_meta_stringdata_MotionDetect = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MotionDetect"
QT_MOC_LITERAL(1, 13, 8), // "detected"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 11), // "motionCount"
QT_MOC_LITERAL(4, 35, 6), // "detect"
QT_MOC_LITERAL(5, 42, 23), // "QSharedPointer<uint8_t>"
QT_MOC_LITERAL(6, 66, 4), // "data"
QT_MOC_LITERAL(7, 71, 5), // "width"
QT_MOC_LITERAL(8, 77, 6) // "height"

    },
    "MotionDetect\0detected\0\0motionCount\0"
    "detect\0QSharedPointer<uint8_t>\0data\0"
    "width\0height"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MotionDetect[] = {

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
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    3,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int, QMetaType::Int,    6,    7,    8,

       0        // eod
};

void MotionDetect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MotionDetect *_t = static_cast<MotionDetect *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->detected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->detect((*reinterpret_cast< QSharedPointer<uint8_t>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MotionDetect::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MotionDetect::detected)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MotionDetect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MotionDetect.data,
      qt_meta_data_MotionDetect,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MotionDetect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MotionDetect::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MotionDetect.stringdata0))
        return static_cast<void*>(const_cast< MotionDetect*>(this));
    return QObject::qt_metacast(_clname);
}

int MotionDetect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void MotionDetect::detected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
