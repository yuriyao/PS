/****************************************************************************
** Meta object code from reading C++ file 'filemenu.h'
**
** Created: Mon Jan 7 11:39:05 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/browser/filemenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filemenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileChooser[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_FileChooser[] = {
    "FileChooser\0"
};

void FileChooser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData FileChooser::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileChooser::staticMetaObject = {
    { &QFileDialog::staticMetaObject, qt_meta_stringdata_FileChooser,
      qt_meta_data_FileChooser, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileChooser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileChooser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileChooser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileChooser))
        return static_cast<void*>(const_cast< FileChooser*>(this));
    return QFileDialog::qt_metacast(_clname);
}

int FileChooser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFileDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_FileMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   10,    9,    9, 0x05,
      59,   50,    9,    9, 0x05,
      86,    9,    9,    9, 0x05,
     110,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     124,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FileMenu[] = {
    "FileMenu\0\0location\0open_location_signal(QString&)\0"
    "filename\0open_file_signal(QString&)\0"
    "create_new_tab_signal()\0exit_signal()\0"
    "open_file_slot()\0"
};

void FileMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileMenu *_t = static_cast<FileMenu *>(_o);
        switch (_id) {
        case 0: _t->open_location_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->open_file_signal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->create_new_tab_signal(); break;
        case 3: _t->exit_signal(); break;
        case 4: _t->open_file_slot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileMenu::staticMetaObject = {
    { &QMenu::staticMetaObject, qt_meta_stringdata_FileMenu,
      qt_meta_data_FileMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileMenu))
        return static_cast<void*>(const_cast< FileMenu*>(this));
    return QMenu::qt_metacast(_clname);
}

int FileMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void FileMenu::open_location_signal(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileMenu::open_file_signal(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileMenu::create_new_tab_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FileMenu::exit_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
