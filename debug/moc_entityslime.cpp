/****************************************************************************
** Meta object code from reading C++ file 'entityslime.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../model/entityslime.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'entityslime.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_parkour__EntitySlime_t {
    QByteArrayData data[3];
    char stringdata0[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_parkour__EntitySlime_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_parkour__EntitySlime_t qt_meta_stringdata_parkour__EntitySlime = {
    {
QT_MOC_LITERAL(0, 0, 20), // "parkour::EntitySlime"
QT_MOC_LITERAL(1, 21, 11), // "EntitySlime"
QT_MOC_LITERAL(2, 33, 0) // ""

    },
    "parkour::EntitySlime\0EntitySlime\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_parkour__EntitySlime[] = {

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

void parkour::EntitySlime::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { EntitySlime *_r = new EntitySlime();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    }
    Q_UNUSED(_o);
}

QT_INIT_METAOBJECT const QMetaObject parkour::EntitySlime::staticMetaObject = { {
    QMetaObject::SuperData::link<EntityPlayerLike::staticMetaObject>(),
    qt_meta_stringdata_parkour__EntitySlime.data,
    qt_meta_data_parkour__EntitySlime,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *parkour::EntitySlime::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *parkour::EntitySlime::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_parkour__EntitySlime.stringdata0))
        return static_cast<void*>(this);
    return EntityPlayerLike::qt_metacast(_clname);
}

int parkour::EntitySlime::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = EntityPlayerLike::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
