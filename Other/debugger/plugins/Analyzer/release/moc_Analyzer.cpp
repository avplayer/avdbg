/****************************************************************************
** Meta object code from reading C++ file 'Analyzer.h'
**
** Created: Sat Sep 15 17:40:50 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Analyzer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Analyzer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Analyzer[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       7,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      20,    9,
      50,   27,

 // signals: signature, parameters, type, tag, flags
      55,   54,   54,   54, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   54,   54,   54, 0x0a,
      93,   54,   54,   54, 0x0a,
     112,   54,   54,   54, 0x0a,
     134,   54,   54,   54, 0x0a,
     154,   54,   54,   54, 0x0a,
     176,   54,   54,   54, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Analyzer[] = {
    "Analyzer\0Evan Teran\0author\0"
    "http://www.codef00.com\0url\0\0"
    "update_progress(int)\0do_ip_analysis()\0"
    "do_view_analysis()\0goto_function_start()\0"
    "goto_function_end()\0mark_function_start()\0"
    "show_specified()\0"
};

void Analyzer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Analyzer *_t = static_cast<Analyzer *>(_o);
        switch (_id) {
        case 0: _t->update_progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->do_ip_analysis(); break;
        case 2: _t->do_view_analysis(); break;
        case 3: _t->goto_function_start(); break;
        case 4: _t->goto_function_end(); break;
        case 5: _t->mark_function_start(); break;
        case 6: _t->show_specified(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Analyzer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Analyzer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Analyzer,
      qt_meta_data_Analyzer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Analyzer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Analyzer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Analyzer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Analyzer))
        return static_cast<void*>(const_cast< Analyzer*>(this));
    if (!strcmp(_clname, "AnalyzerInterface"))
        return static_cast< AnalyzerInterface*>(const_cast< Analyzer*>(this));
    if (!strcmp(_clname, "DebuggerPluginInterface"))
        return static_cast< DebuggerPluginInterface*>(const_cast< Analyzer*>(this));
    if (!strcmp(_clname, "EDB.DebuggerPluginInterface/1.0"))
        return static_cast< DebuggerPluginInterface*>(const_cast< Analyzer*>(this));
    return QObject::qt_metacast(_clname);
}

int Analyzer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Analyzer::update_progress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
