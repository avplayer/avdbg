/****************************************************************************
** Meta object code from reading C++ file 'qhexview.h'
**
** Created: Sat Sep 15 17:40:25 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qhexview/qhexview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qhexview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QHexView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      31,    9,    9,    9, 0x0a,
      54,    9,    9,    9, 0x0a,
      75,    9,    9,    9, 0x0a,
      97,    9,    9,    9, 0x0a,
     115,    9,    9,    9, 0x0a,
     137,  132,    9,    9, 0x0a,
     158,  152,    9,    9, 0x0a,
     188,    9,    9,    9, 0x0a,
     198,    9,    9,    9, 0x0a,
     206,    9,    9,    9, 0x0a,
     218,    9,    9,    9, 0x0a,
     234,    9,  229,    9, 0x0a,
     252,    9,    9,    9, 0x0a,
     265,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QHexView[] = {
    "QHexView\0\0setShowAddress(bool)\0"
    "setShowAsciiDump(bool)\0setShowHexDump(bool)\0"
    "setShowComments(bool)\0setWordWidth(int)\0"
    "setRowWidth(int)\0font\0setFont(QFont)\0"
    "value\0setShowAddressSeparator(bool)\0"
    "repaint()\0clear()\0selectAll()\0deselect()\0"
    "bool\0hasSelectedText()\0mnuSetFont()\0"
    "mnuCopy()\0"
};

void QHexView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QHexView *_t = static_cast<QHexView *>(_o);
        switch (_id) {
        case 0: _t->setShowAddress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setShowAsciiDump((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->setShowHexDump((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setShowComments((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setWordWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setRowWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setFont((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 7: _t->setShowAddressSeparator((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->repaint(); break;
        case 9: _t->clear(); break;
        case 10: _t->selectAll(); break;
        case 11: _t->deselect(); break;
        case 12: { bool _r = _t->hasSelectedText();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: _t->mnuSetFont(); break;
        case 14: _t->mnuCopy(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QHexView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QHexView::staticMetaObject = {
    { &QAbstractScrollArea::staticMetaObject, qt_meta_stringdata_QHexView,
      qt_meta_data_QHexView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QHexView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QHexView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QHexView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QHexView))
        return static_cast<void*>(const_cast< QHexView*>(this));
    return QAbstractScrollArea::qt_metacast(_clname);
}

int QHexView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
