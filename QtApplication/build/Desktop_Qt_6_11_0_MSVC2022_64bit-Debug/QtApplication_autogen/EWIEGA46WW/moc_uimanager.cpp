/****************************************************************************
** Meta object code from reading C++ file 'uimanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../uimanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uimanager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9UIManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto UIManager::qt_create_metaobjectdata<qt_meta_tag_ZN9UIManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UIManager",
        "0$cVBdV5OqIIZrTyavzyVaN5TBQDs",
        "startGame",
        "",
        "keyReleaseSig",
        "QKeyEvent*",
        "event",
        "keyPressSig",
        "potIsTurning",
        "potStops",
        "shootPressed",
        "shootReleased",
        "keyPressEvent",
        "keyReleaseEvent"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'startGame'
        QtMocHelpers::SignalData<void()>(2, 3, QMC::AccessPublic, QMetaType::Void),
        // Signal 'keyReleaseSig'
        QtMocHelpers::SignalData<void(QKeyEvent *)>(4, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Signal 'keyPressSig'
        QtMocHelpers::SignalData<void(QKeyEvent *)>(7, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Slot 'potIsTurning'
        QtMocHelpers::SlotData<void()>(8, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'potStops'
        QtMocHelpers::SlotData<void()>(9, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'shootPressed'
        QtMocHelpers::SlotData<void()>(10, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'shootReleased'
        QtMocHelpers::SlotData<void()>(11, 3, QMC::AccessPublic, QMetaType::Void),
        // Slot 'keyPressEvent'
        QtMocHelpers::SlotData<void(QKeyEvent *)>(12, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
        // Slot 'keyReleaseEvent'
        QtMocHelpers::SlotData<void(QKeyEvent *)>(13, 3, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    uint qt_metaObjectHashIndex = 1;
    return QtMocHelpers::metaObjectData<UIManager, qt_meta_tag_ZN9UIManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_metaObjectHashIndex);
}
Q_CONSTINIT const QMetaObject UIManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UIManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UIManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9UIManagerE_t>.metaTypes,
    nullptr
} };

void UIManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UIManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->startGame(); break;
        case 1: _t->keyReleaseSig((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 2: _t->keyPressSig((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 3: _t->potIsTurning(); break;
        case 4: _t->potStops(); break;
        case 5: _t->shootPressed(); break;
        case 6: _t->shootReleased(); break;
        case 7: _t->keyPressEvent((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 8: _t->keyReleaseEvent((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (UIManager::*)()>(_a, &UIManager::startGame, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (UIManager::*)(QKeyEvent * )>(_a, &UIManager::keyReleaseSig, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (UIManager::*)(QKeyEvent * )>(_a, &UIManager::keyPressSig, 2))
            return;
    }
}

const QMetaObject *UIManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UIManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9UIManagerE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int UIManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void UIManager::startGame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UIManager::keyReleaseSig(QKeyEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void UIManager::keyPressSig(QKeyEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
