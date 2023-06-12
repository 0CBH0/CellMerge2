/****************************************************************************
** Meta object code from reading C++ file 'LevelSetting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../LevelSetting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LevelSetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LevelSetting_t {
    QByteArrayData data[13];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LevelSetting_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LevelSetting_t qt_meta_stringdata_LevelSetting = {
    {
QT_MOC_LITERAL(0, 0, 12), // "LevelSetting"
QT_MOC_LITERAL(1, 13, 12), // "valueChanged"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "left"
QT_MOC_LITERAL(4, 32, 3), // "mid"
QT_MOC_LITERAL(5, 36, 5), // "right"
QT_MOC_LITERAL(6, 42, 15), // "setCurrentRange"
QT_MOC_LITERAL(7, 58, 11), // "setMidValue"
QT_MOC_LITERAL(8, 70, 12), // "setLeftValue"
QT_MOC_LITERAL(9, 83, 13), // "setRightValue"
QT_MOC_LITERAL(10, 97, 8), // "midValue"
QT_MOC_LITERAL(11, 106, 9), // "leftValue"
QT_MOC_LITERAL(12, 116, 10) // "rightValue"

    },
    "LevelSetting\0valueChanged\0\0left\0mid\0"
    "right\0setCurrentRange\0setMidValue\0"
    "setLeftValue\0setRightValue\0midValue\0"
    "leftValue\0rightValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LevelSetting[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       3,   62, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    3,   46,    2, 0x0a /* Public */,
       7,    1,   53,    2, 0x0a /* Public */,
       8,    1,   56,    2, 0x0a /* Public */,
       9,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Int,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,

 // properties: name, type, flags
      10, QMetaType::Int, 0x00095103,
      11, QMetaType::Int, 0x00095103,
      12, QMetaType::Int, 0x00095103,

       0        // eod
};

void LevelSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LevelSetting *_t = static_cast<LevelSetting *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->setCurrentRange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->setMidValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setLeftValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setRightValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LevelSetting::*_t)(int , double , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LevelSetting::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        LevelSetting *_t = static_cast<LevelSetting *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getMidValue(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getLeftValue(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->getRightValue(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        LevelSetting *_t = static_cast<LevelSetting *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMidValue(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setLeftValue(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setRightValue(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject LevelSetting::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LevelSetting.data,
      qt_meta_data_LevelSetting,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LevelSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LevelSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LevelSetting.stringdata0))
        return static_cast<void*>(const_cast< LevelSetting*>(this));
    return QWidget::qt_metacast(_clname);
}

int LevelSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void LevelSetting::valueChanged(int _t1, double _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
