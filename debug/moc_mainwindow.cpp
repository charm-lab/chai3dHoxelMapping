/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[279];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "UpdateGUIInfo"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "onGUIchanged"
QT_MOC_LITERAL(4, 39, 18), // "on_turnOff_clicked"
QT_MOC_LITERAL(5, 58, 15), // "WriteDataToFile"
QT_MOC_LITERAL(6, 74, 29), // "on_dynamicEnvironment_clicked"
QT_MOC_LITERAL(7, 104, 20), // "rotateTissueLineDisp"
QT_MOC_LITERAL(8, 125, 5), // "angle"
QT_MOC_LITERAL(9, 131, 12), // "CheckFingers"
QT_MOC_LITERAL(10, 144, 19), // "RecordCDInertiaData"
QT_MOC_LITERAL(11, 164, 14), // "readExpStuffIn"
QT_MOC_LITERAL(12, 179, 23), // "on_StiffnessExp_clicked"
QT_MOC_LITERAL(13, 203, 29), // "on_StiffnMassCombined_clicked"
QT_MOC_LITERAL(14, 233, 27), // "on_FingerMappingExp_clicked"
QT_MOC_LITERAL(15, 261, 17) // "on_Manual_clicked"

    },
    "MainWindow\0UpdateGUIInfo\0\0onGUIchanged\0"
    "on_turnOff_clicked\0WriteDataToFile\0"
    "on_dynamicEnvironment_clicked\0"
    "rotateTissueLineDisp\0angle\0CheckFingers\0"
    "RecordCDInertiaData\0readExpStuffIn\0"
    "on_StiffnessExp_clicked\0"
    "on_StiffnMassCombined_clicked\0"
    "on_FingerMappingExp_clicked\0"
    "on_Manual_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    1,   84,    2, 0x08 /* Private */,
       9,    0,   87,    2, 0x08 /* Private */,
      10,    0,   88,    2, 0x08 /* Private */,
      11,    0,   89,    2, 0x08 /* Private */,
      12,    0,   90,    2, 0x08 /* Private */,
      13,    0,   91,    2, 0x08 /* Private */,
      14,    0,   92,    2, 0x08 /* Private */,
      15,    0,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    8,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateGUIInfo(); break;
        case 1: _t->onGUIchanged(); break;
        case 2: _t->on_turnOff_clicked(); break;
        case 3: _t->WriteDataToFile(); break;
        case 4: _t->on_dynamicEnvironment_clicked(); break;
        case 5: _t->rotateTissueLineDisp((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: { bool _r = _t->CheckFingers();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->RecordCDInertiaData(); break;
        case 8: { bool _r = _t->readExpStuffIn();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: _t->on_StiffnessExp_clicked(); break;
        case 10: _t->on_StiffnMassCombined_clicked(); break;
        case 11: _t->on_FingerMappingExp_clicked(); break;
        case 12: _t->on_Manual_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
