/****************************************************************************
** Meta object code from reading C++ file 'entityfireball.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../model/entityfireball.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'entityfireball.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_parkour__EntityFireball_t {
    QByteArrayData data[4];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_parkour__EntityFireball_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_parkour__EntityFireball_t qt_meta_stringdata_parkour__EntityFireball = {
    {
QT_MOC_LITERAL(0, 0, 23), // "parkour::EntityFireball"
QT_MOC_LITERAL(1, 24, 14), // "EntityFireball"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5) // "power"

    },
    "parkour::EntityFireball\0EntityFireball\0"
    "\0power"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_parkour__EntityFireball[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       2,   18, // constructors
       0,       // flags
       0,       // signalCount

 // constructors: parameters
    0x80000000 | 2, QMetaType::Double,    3,
    0x80000000 | 2,

 // constructors: name, argc, parameters, tag, flags
       1,    1,   14,    2, 0x0e /* Public */,
       1,    0,   17,    2, 0x2e /* Public | MethodCloned */,

       0        // eod
};

void parkour::EntityFireball::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { EntityFireball *_r = new EntityFireball((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        case 1: { EntityFireball *_r = new EntityFireball();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    }
    Q_UNUSED(_o);
}

QT_INIT_METAOBJECT const QMetaObject parkour::EntityFireball::staticMetaObject = { {
    QMetaObject::SuperData::link<Entity::staticMetaObject>(),
    qt_meta_stringdata_parkour__EntityFireball.data,
    qt_meta_data_parkour__EntityFireball,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *parkour::EntityFireball::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *parkour::EntityFireball::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_parkour__EntityFireball.stringdata0))
        return static_cast<void*>(this);
    return Entity::qt_metacast(_clname);
}

int parkour::EntityFireball::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Entity::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
