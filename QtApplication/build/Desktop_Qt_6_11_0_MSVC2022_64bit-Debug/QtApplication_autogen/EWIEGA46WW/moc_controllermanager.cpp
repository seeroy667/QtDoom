/****************************************************************************
** Meta object code from reading C++ file 'controllermanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../controllermanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controllermanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN17ControllerManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto ControllerManager::qt_create_metaobjectdata<qt_meta_tag_ZN17ControllerManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ControllerManager",
        "0$M9rRYbarOq3+G9IaHW9EnHJodsI",
        "potTurnedSig",
        "",
        "potStopedSig",
        "shootPressedSig",
        "shootReleasedSig",
        "keyPressedEvent",
        "QKeyEvent*",
        "event",
        "keyReleasedEvent",
        "potTurnedEvent",
        "potStopedEvent",
        "shootPressedEvent",
        "shootReleasedEvent"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'potTurnedSig'
        QtMocHelpers::SignalData<void()>(2, 3, QMC::AccessPublic, QMetaType::Void),
        // Signal 'potStopedSig'
        QtMocHelpers::SignalData<void()>(4, 3, QMC::AccessPublic, QMetaType::Void),
        // Signal 'shootPressedSig'
        QtMocHelpers::SignalData<void()>(5, 3, QMC::AccessPublic, QMetaType::Void),
        // Signal 'shootReleasedSig'
        QtMocHelpers::SignalData<void()>(6, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'keyPressedEvent'
        QtMocHelpers::SlotData<void(QKeyEvent *)>(7, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'keyReleasedEvent'
        QtMocHelpers::SlotData<void(QKeyEvent *)>(10, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Slot 'potTurnedEvent'
        QtMocHelpers::SlotData<void()>(11, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'potStopedEvent'
        QtMocHelpers::SlotData<void()>(12, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'shootPressedEvent'
        QtMocHelpers::SlotData<void()>(13, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'shootReleasedEvent'
        QtMocHelpers::SlotData<void()>(14, 3, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    uint qt_metaObjectHashIndex = 1;
    return QtMocHelpers::metaObjectData<ControllerManager, qt_meta_tag_ZN17ControllerManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_metaObjectHashIndex);
}
Q_CONSTINIT const QMetaObject ControllerManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ControllerManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ControllerManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17ControllerManagerE_t>.metaTypes,
    nullptr
} };

void ControllerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ControllerManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->potTurnedSig(); break;
        case 1: _t->potStopedSig(); break;
        case 2: _t->shootPressedSig(); break;
        case 3: _t->shootReleasedSig(); break;
        case 4: _t->keyPressedEvent((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 5: _t->keyReleasedEvent((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 6: _t->potTurnedEvent(); break;
        case 7: _t->potStopedEvent(); break;
        case 8: _t->shootPressedEvent(); break;
        case 9: _t->shootReleasedEvent(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ControllerManager::*)()>(_a, &ControllerManager::potTurnedSig, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControllerManager::*)()>(_a, &ControllerManager::potStopedSig, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControllerManager::*)()>(_a, &ControllerManager::shootPressedSig, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ControllerManager::*)()>(_a, &ControllerManager::shootReleasedSig, 3))
            return;
    }
}

const QMetaObject *ControllerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControllerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ControllerManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ControllerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ControllerManager::potTurnedSig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ControllerManager::potStopedSig()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ControllerManager::shootPressedSig()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ControllerManager::shootReleasedSig()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
