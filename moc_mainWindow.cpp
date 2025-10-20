/****************************************************************************
** Meta object code from reading C++ file 'mainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[14];
    char stringdata0[283];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 17), // "slot_updateModels"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 19), // "slot_itemDialog_add"
QT_MOC_LITERAL(4, 50, 20), // "slot_itemDialog_edit"
QT_MOC_LITERAL(5, 71, 26), // "slot_editItemByDoubleClick"
QT_MOC_LITERAL(6, 98, 11), // "QModelIndex"
QT_MOC_LITERAL(7, 110, 20), // "slot_itemDialog_copy"
QT_MOC_LITERAL(8, 131, 26), // "slot_itemsModelView_remove"
QT_MOC_LITERAL(9, 158, 21), // "slot_incomeDialog_add"
QT_MOC_LITERAL(10, 180, 22), // "slot_incomeDialog_edit"
QT_MOC_LITERAL(11, 203, 28), // "slot_editIncomeByDoubleClick"
QT_MOC_LITERAL(12, 232, 22), // "slot_incomeDialog_copy"
QT_MOC_LITERAL(13, 255, 27) // "slot_incomeModelView_remove"

    },
    "MainWindow\0slot_updateModels\0\0"
    "slot_itemDialog_add\0slot_itemDialog_edit\0"
    "slot_editItemByDoubleClick\0QModelIndex\0"
    "slot_itemDialog_copy\0slot_itemsModelView_remove\0"
    "slot_incomeDialog_add\0slot_incomeDialog_edit\0"
    "slot_editIncomeByDoubleClick\0"
    "slot_incomeDialog_copy\0"
    "slot_incomeModelView_remove"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    1,   72,    2, 0x08 /* Private */,
       7,    0,   75,    2, 0x08 /* Private */,
       8,    0,   76,    2, 0x08 /* Private */,
       9,    0,   77,    2, 0x08 /* Private */,
      10,    0,   78,    2, 0x08 /* Private */,
      11,    1,   79,    2, 0x08 /* Private */,
      12,    0,   82,    2, 0x08 /* Private */,
      13,    0,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slot_updateModels(); break;
        case 1: _t->slot_itemDialog_add(); break;
        case 2: _t->slot_itemDialog_edit(); break;
        case 3: _t->slot_editItemByDoubleClick((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 4: _t->slot_itemDialog_copy(); break;
        case 5: _t->slot_itemsModelView_remove(); break;
        case 6: _t->slot_incomeDialog_add(); break;
        case 7: _t->slot_incomeDialog_edit(); break;
        case 8: _t->slot_editIncomeByDoubleClick((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 9: _t->slot_incomeDialog_copy(); break;
        case 10: _t->slot_incomeModelView_remove(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
