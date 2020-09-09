/****************************************************************************
** Meta object code from reading C++ file 'entitytnt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../model/entitytnt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'entitytnt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_parkour__EntityTNT_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_parkour__EntityTNT_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_parkour__EntityTNT_t qt_meta_stringdata_parkour__EntityTNT = {
    {
QT_MOC_LITERAL(0, 0, 18), // "parkour::EntityTNT"
QT_MOC_LITERAL(1, 19, 9), // "EntityTNT"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "parkour::EntityTNT\0EntityTNT\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_parkour__EntityTNT[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       1,   15, // constructors
       0,       // flags
       0,       // signalCount

 // constructors: parameters
    0x80000000 | 2,

 // constructors: name, argc, parameters, tag, flags
       1,    0,   14,    2, 0x0e /* Public */,

       0        // eod
};

void parkour::EntityTNT::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { EntityTNT *_r = new EntityTNT();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    }
    Q_UNUSED(_o);
}

QT_INIT_METAOBJECT const QMetaObject parkour::EntityTNT::staticMetaObject = { {
    QMetaObject::SuperData::link<Entity::staticMetaObject>(),
    qt_meta_stringdata_parkour__EntityTNT.data,
    qt_meta_data_parkour__EntityTNT,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *parkour::EntityTNT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *parkour::EntityTNT::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_parkour__EntityTNT.stringdata0))
        return static_cast<void*>(this);
    return Entity::qt_metacast(_clname);
}

int parkour::EntityTNT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Entity::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
