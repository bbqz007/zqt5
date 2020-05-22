/****************************************************************************
** Meta object code from reading C++ file 'zqt_helper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#error "run moc zqt_helper.h > zqt_helper_moc.h, other remove this line"

#include <memory>
#include "zqt_helper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'zqt_helper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_zhelper__qt5Widgets__ZQEmitter_t {
    QByteArrayData data[9];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_zhelper__qt5Widgets__ZQEmitter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_zhelper__qt5Widgets__ZQEmitter_t qt_meta_stringdata_zhelper__qt5Widgets__ZQEmitter = {
    {
QT_MOC_LITERAL(0, 0, 30), // "zhelper::qt5Widgets::ZQEmitter"
QT_MOC_LITERAL(1, 31, 10), // "leaveEvent"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 7), // "QEvent*"
QT_MOC_LITERAL(4, 51, 5), // "event"
QT_MOC_LITERAL(5, 57, 9), // "dropEvent"
QT_MOC_LITERAL(6, 67, 11), // "QDropEvent*"
QT_MOC_LITERAL(7, 79, 14), // "dragEnterEvent"
QT_MOC_LITERAL(8, 94, 16) // "QDragEnterEvent*"

    },
    "zhelper::qt5Widgets::ZQEmitter\0"
    "leaveEvent\0\0QEvent*\0event\0dropEvent\0"
    "QDropEvent*\0dragEnterEvent\0QDragEnterEvent*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_zhelper__qt5Widgets__ZQEmitter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    1,   32,    2, 0x06 /* Public */,
       7,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    4,

       0        // eod
};

void zhelper::qt5Widgets::ZQEmitter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ZQEmitter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->leaveEvent((*reinterpret_cast< QEvent*(*)>(_a[1]))); break;
        case 1: _t->dropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 2: _t->dragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ZQEmitter::*)(QEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ZQEmitter::leaveEvent)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ZQEmitter::*)(QDropEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ZQEmitter::dropEvent)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ZQEmitter::*)(QDragEnterEvent * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ZQEmitter::dragEnterEvent)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject zhelper::qt5Widgets::ZQEmitter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_zhelper__qt5Widgets__ZQEmitter.data,
    qt_meta_data_zhelper__qt5Widgets__ZQEmitter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *zhelper::qt5Widgets::ZQEmitter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *zhelper::qt5Widgets::ZQEmitter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_zhelper__qt5Widgets__ZQEmitter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int zhelper::qt5Widgets::ZQEmitter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void zhelper::qt5Widgets::ZQEmitter::leaveEvent(QEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void zhelper::qt5Widgets::ZQEmitter::dropEvent(QDropEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void zhelper::qt5Widgets::ZQEmitter::dragEnterEvent(QDragEnterEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
