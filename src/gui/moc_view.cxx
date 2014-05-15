/****************************************************************************
** Meta object code from reading C++ file 'view.hpp'
**
** Created: Mon Mar 10 17:27:37 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PointSetView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   14,   13,   13, 0x05,
      53,   51,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   13,   13,   13, 0x0a,
      86,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PointSetView[] = {
    "PointSetView\0\0x,y\0pointsetCursorMoved(float,float)\0"
    "z\0zoomChanged(float)\0timeOutSlot()\0"
    "setZoom(double)\0"
};

void PointSetView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PointSetView *_t = static_cast<PointSetView *>(_o);
        switch (_id) {
        case 0: _t->pointsetCursorMoved((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->zoomChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->timeOutSlot(); break;
        case 3: _t->setZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PointSetView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PointSetView::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_PointSetView,
      qt_meta_data_PointSetView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PointSetView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PointSetView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PointSetView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PointSetView))
        return static_cast<void*>(const_cast< PointSetView*>(this));
    if (!strcmp(_clname, "QGLFunctions"))
        return static_cast< QGLFunctions*>(const_cast< PointSetView*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int PointSetView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void PointSetView::pointsetCursorMoved(float _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PointSetView::zoomChanged(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
