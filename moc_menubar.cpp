/****************************************************************************
** Meta object code from reading C++ file 'menubar.h'
**
** Created: Mon Jan 7 11:45:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/browser/menubar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'menubar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MenuBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,    9,    8,    8, 0x05,
      58,   49,    8,    8, 0x05,
      85,    8,    8,    8, 0x05,
     109,    8,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MenuBar[] = {
    "MenuBar\0\0location\0open_location_signal(QString&)\0"
    "filename\0open_file_signal(QString&)\0"
    "create_new_tab_signal()\0exit_signal()\0"
};

void MenuBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MenuBar *_t = static_cast<MenuBar *>(_o);
        switch (_id) {
        case 0: _t->open_location_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->open_file_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->create_new_tab_signal(); break;
        case 3: _t->exit_signal(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MenuBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MenuBar::staticMetaObject = {
    { &QMenuBar::staticMetaObject, qt_meta_stringdata_MenuBar,
      qt_meta_data_MenuBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MenuBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MenuBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MenuBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MenuBar))
        return static_cast<void*>(const_cast< MenuBar*>(this));
    return QMenuBar::qt_metacast(_clname);
}

int MenuBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenuBar::qt_metacall(_c, _id, _a);
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
void MenuBar::open_location_signal(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MenuBar::open_file_signal(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MenuBar::create_new_tab_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void MenuBar::exit_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
