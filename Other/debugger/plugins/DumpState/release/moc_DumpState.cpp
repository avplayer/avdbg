/****************************************************************************
** Meta object code from reading C++ file 'DumpState.h'
**
** Created: Sat Sep 15 17:41:14 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../DumpState.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DumpState.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DumpState[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       1,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
      21,   10,
      51,   28,

 // slots: signature, parameters, type, tag, flags
      56,   55,   55,   55, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DumpState[] = {
    "DumpState\0Evan Teran\0author\0"
    "http://www.codef00.com\0url\0\0show_menu()\0"
};

void DumpState::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DumpState *_t = static_cast<DumpState *>(_o);
        switch (_id) {
        case 0: _t->show_menu(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DumpState::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DumpState::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DumpState,
      qt_meta_data_DumpState, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DumpState::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DumpState::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DumpState::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DumpState))
        return static_cast<void*>(const_cast< DumpState*>(this));
    if (!strcmp(_clname, "DebuggerPluginInterface"))
        return static_cast< DebuggerPluginInterface*>(const_cast< DumpState*>(this));
    if (!strcmp(_clname, "EDB.DebuggerPluginInterface/1.0"))
        return static_cast< DebuggerPluginInterface*>(const_cast< DumpState*>(this));
    return QObject::qt_metacast(_clname);
}

int DumpState::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
