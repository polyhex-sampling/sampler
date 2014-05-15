/****************************************************************************
** Meta object code from reading C++ file 'window.hpp'
**
** Created: Mon Mar 10 17:27:37 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "window.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x0a,
      30,   12,   11,   11, 0x0a,
      44,   11,   11,   11, 0x0a,
      59,   11,   11,   11, 0x0a,
      75,   11,   11,   11, 0x0a,
      96,   91,   11,   11, 0x0a,
     109,   11,   11,   11, 0x0a,
     128,  122,   11,   11, 0x0a,
     145,  122,   11,   11, 0x0a,
     167,  162,   11,   11, 0x0a,
     183,  162,   11,   11, 0x0a,
     205,  199,   11,   11, 0x0a,
     231,  199,   11,   11, 0x0a,
     246,   12,   11,   11, 0x0a,
     270,  266,   11,   11, 0x0a,
     292,   12,   11,   11, 0x0a,
     312,   12,   11,   11, 0x0a,
     331,  199,   11,   11, 0x0a,
     344,  199,   11,   11, 0x0a,
     361,  357,   11,   11, 0x0a,
     390,   11,   11,   11, 0x0a,
     409,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0index\0getLut(int)\0"
    "getDFunc(int)\0getImageFile()\0"
    "getDensityMin()\0getDensityMax()\0seed\0"
    "getSeed(int)\0getSpaceSz()\0level\0"
    "getLevelMin(int)\0getLevelMax(int)\0"
    "rank\0getRankMin(int)\0getRankMax(int)\0"
    "state\0getOffsetInterpolate(int)\0"
    "getDither(int)\0getSampleShape(int)\0"
    "rad\0getDiskRadius(double)\0getSampleColor(int)\0"
    "getBackground(int)\0getCrop(int)\0"
    "getAuto(int)\0x,y\0showCoordinates(float,float)\0"
    "generatePointset()\0updatePointset()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->getLut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->getDFunc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->getImageFile(); break;
        case 3: _t->getDensityMin(); break;
        case 4: _t->getDensityMax(); break;
        case 5: _t->getSeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->getSpaceSz(); break;
        case 7: _t->getLevelMin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->getLevelMax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->getRankMin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->getRankMax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->getOffsetInterpolate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->getDither((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->getSampleShape((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->getDiskRadius((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 15: _t->getSampleColor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->getBackground((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->getCrop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->getAuto((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->showCoordinates((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 20: _t->generatePointset(); break;
        case 21: _t->updatePointset(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
