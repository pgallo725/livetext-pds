/****************************************************************************
** Meta object code from reading C++ file 'MessageHandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MessageHandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MessageHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MessageHandler_t {
    QByteArrayData data[30];
    char stringdata0[314];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MessageHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MessageHandler_t qt_meta_stringdata_MessageHandler = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MessageHandler"
QT_MOC_LITERAL(1, 15, 12), // "loginRequest"
QT_MOC_LITERAL(2, 28, 14), // "MessageCapsule"
QT_MOC_LITERAL(3, 43, 0), // ""
QT_MOC_LITERAL(4, 44, 11), // "QTcpSocket*"
QT_MOC_LITERAL(5, 56, 12), // "clientSocket"
QT_MOC_LITERAL(6, 69, 8), // "username"
QT_MOC_LITERAL(7, 78, 11), // "loginUnlock"
QT_MOC_LITERAL(8, 90, 5), // "token"
QT_MOC_LITERAL(9, 96, 13), // "accountCreate"
QT_MOC_LITERAL(10, 110, 11), // "lientSocket"
QT_MOC_LITERAL(11, 122, 5), // "User&"
QT_MOC_LITERAL(12, 128, 7), // "newUser"
QT_MOC_LITERAL(13, 136, 13), // "accountUpdate"
QT_MOC_LITERAL(14, 150, 11), // "updatedUser"
QT_MOC_LITERAL(15, 162, 14), // "documentCreate"
QT_MOC_LITERAL(16, 177, 7), // "docName"
QT_MOC_LITERAL(17, 185, 12), // "documentOpen"
QT_MOC_LITERAL(18, 198, 6), // "docUri"
QT_MOC_LITERAL(19, 205, 14), // "documentRemove"
QT_MOC_LITERAL(20, 220, 10), // "charInsert"
QT_MOC_LITERAL(21, 231, 7), // "Symbol&"
QT_MOC_LITERAL(22, 239, 1), // "s"
QT_MOC_LITERAL(23, 241, 10), // "charDelete"
QT_MOC_LITERAL(24, 252, 15), // "QVector<qint32>"
QT_MOC_LITERAL(25, 268, 3), // "pos"
QT_MOC_LITERAL(26, 272, 15), // "messageDispatch"
QT_MOC_LITERAL(27, 288, 7), // "message"
QT_MOC_LITERAL(28, 296, 6), // "sender"
QT_MOC_LITERAL(29, 303, 10) // "userLogout"

    },
    "MessageHandler\0loginRequest\0MessageCapsule\0"
    "\0QTcpSocket*\0clientSocket\0username\0"
    "loginUnlock\0token\0accountCreate\0"
    "lientSocket\0User&\0newUser\0accountUpdate\0"
    "updatedUser\0documentCreate\0docName\0"
    "documentOpen\0docUri\0documentRemove\0"
    "charInsert\0Symbol&\0s\0charDelete\0"
    "QVector<qint32>\0pos\0messageDispatch\0"
    "message\0sender\0userLogout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MessageHandler[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    3, 0x06 /* Public */,
       7,    2,   74,    3, 0x06 /* Public */,
       9,    2,   79,    3, 0x06 /* Public */,
      13,    2,   84,    3, 0x06 /* Public */,
      15,    2,   89,    3, 0x06 /* Public */,
      17,    2,   94,    3, 0x06 /* Public */,
      19,    2,   99,    3, 0x06 /* Public */,
      20,    1,  104,    3, 0x06 /* Public */,
      23,    1,  107,    3, 0x06 /* Public */,
      26,    2,  110,    3, 0x06 /* Public */,
      29,    1,  115,    3, 0x06 /* Public */,

 // signals: parameters
    0x80000000 | 2, 0x80000000 | 4, QMetaType::QString,    5,    6,
    0x80000000 | 2, 0x80000000 | 4, QMetaType::QString,    5,    8,
    0x80000000 | 2, 0x80000000 | 4, 0x80000000 | 11,   10,   12,
    0x80000000 | 2, 0x80000000 | 4, 0x80000000 | 11,   10,   14,
    0x80000000 | 2, 0x80000000 | 4, QMetaType::QString,   10,   16,
    0x80000000 | 2, 0x80000000 | 4, QMetaType::QString,   10,   18,
    0x80000000 | 2, 0x80000000 | 4, QMetaType::QString,   10,   18,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, 0x80000000 | 2, 0x80000000 | 4,   27,   28,
    0x80000000 | 2, 0x80000000 | 4,    5,

       0        // eod
};

void MessageHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MessageHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { MessageCapsule _r = _t->loginRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 1: { MessageCapsule _r = _t->loginUnlock((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 2: { MessageCapsule _r = _t->accountCreate((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< User(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 3: { MessageCapsule _r = _t->accountUpdate((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< User(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 4: { MessageCapsule _r = _t->documentCreate((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 5: { MessageCapsule _r = _t->documentOpen((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 6: { MessageCapsule _r = _t->documentRemove((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->charInsert((*reinterpret_cast< Symbol(*)>(_a[1]))); break;
        case 8: _t->charDelete((*reinterpret_cast< QVector<qint32>(*)>(_a[1]))); break;
        case 9: _t->messageDispatch((*reinterpret_cast< MessageCapsule(*)>(_a[1])),(*reinterpret_cast< QTcpSocket*(*)>(_a[2]))); break;
        case 10: { MessageCapsule _r = _t->userLogout((*reinterpret_cast< QTcpSocket*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<qint32> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::loginRequest)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::loginUnlock)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * , User & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::accountCreate)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * , User & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::accountUpdate)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::documentCreate)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::documentOpen)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::documentRemove)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MessageHandler::*)(Symbol & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::charInsert)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MessageHandler::*)(QVector<qint32> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::charDelete)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (MessageHandler::*)(MessageCapsule , QTcpSocket * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::messageDispatch)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = MessageCapsule (MessageHandler::*)(QTcpSocket * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MessageHandler::userLogout)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MessageHandler::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MessageHandler.data,
    qt_meta_data_MessageHandler,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MessageHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MessageHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MessageHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MessageHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
MessageCapsule MessageHandler::loginRequest(QTcpSocket * _t1, QString _t2)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}

// SIGNAL 1
MessageCapsule MessageHandler::loginUnlock(QTcpSocket * _t1, QString _t2)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
    return _t0;
}

// SIGNAL 2
MessageCapsule MessageHandler::accountCreate(QTcpSocket * _t1, User & _t2)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
    return _t0;
}

// SIGNAL 3
MessageCapsule MessageHandler::accountUpdate(QTcpSocket * _t1, User & _t2)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
    return _t0;
}

// SIGNAL 4
MessageCapsule MessageHandler::documentCreate(QTcpSocket * _t1, QString _t2)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
    return _t0;
}

// SIGNAL 5
MessageCapsule MessageHandler::documentOpen(QTcpSocket * _t1, QString _t2)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
    return _t0;
}

// SIGNAL 6
MessageCapsule MessageHandler::documentRemove(QTcpSocket * _t1, QString _t2)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
    return _t0;
}

// SIGNAL 7
void MessageHandler::charInsert(Symbol & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MessageHandler::charDelete(QVector<qint32> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MessageHandler::messageDispatch(MessageCapsule _t1, QTcpSocket * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
MessageCapsule MessageHandler::userLogout(QTcpSocket * _t1)
{
    MessageCapsule _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
