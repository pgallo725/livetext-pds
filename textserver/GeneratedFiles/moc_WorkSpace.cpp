/****************************************************************************
** Meta object code from reading C++ file 'WorkSpace.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../WorkSpace.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WorkSpace.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WorkSpace_t {
    QByteArrayData data[49];
    char stringdata0[620];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WorkSpace_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WorkSpace_t qt_meta_stringdata_WorkSpace = {
    {
QT_MOC_LITERAL(0, 0, 9), // "WorkSpace"
QT_MOC_LITERAL(1, 10, 20), // "requestAccountUpdate"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 22), // "QSharedPointer<Client>"
QT_MOC_LITERAL(4, 55, 6), // "client"
QT_MOC_LITERAL(5, 62, 8), // "nickname"
QT_MOC_LITERAL(6, 71, 4), // "icon"
QT_MOC_LITERAL(7, 76, 8), // "password"
QT_MOC_LITERAL(8, 85, 12), // "returnClient"
QT_MOC_LITERAL(9, 98, 16), // "userDisconnected"
QT_MOC_LITERAL(10, 115, 8), // "username"
QT_MOC_LITERAL(11, 124, 9), // "noEditors"
QT_MOC_LITERAL(12, 134, 3), // "URI"
QT_MOC_LITERAL(13, 138, 11), // "documentURI"
QT_MOC_LITERAL(14, 150, 9), // "newClient"
QT_MOC_LITERAL(15, 160, 19), // "clientDisconnection"
QT_MOC_LITERAL(16, 180, 10), // "clientQuit"
QT_MOC_LITERAL(17, 191, 11), // "QSslSocket*"
QT_MOC_LITERAL(18, 203, 12), // "clientSocket"
QT_MOC_LITERAL(19, 216, 8), // "isForced"
QT_MOC_LITERAL(20, 225, 9), // "socketErr"
QT_MOC_LITERAL(21, 235, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(22, 264, 11), // "socketError"
QT_MOC_LITERAL(23, 276, 11), // "readMessage"
QT_MOC_LITERAL(24, 288, 15), // "dispatchMessage"
QT_MOC_LITERAL(25, 304, 14), // "MessageCapsule"
QT_MOC_LITERAL(26, 319, 7), // "message"
QT_MOC_LITERAL(27, 327, 6), // "sender"
QT_MOC_LITERAL(28, 334, 12), // "documentSave"
QT_MOC_LITERAL(29, 347, 20), // "documentInsertSymbol"
QT_MOC_LITERAL(30, 368, 6), // "Symbol"
QT_MOC_LITERAL(31, 375, 6), // "symbol"
QT_MOC_LITERAL(32, 382, 20), // "documentDeleteSymbol"
QT_MOC_LITERAL(33, 403, 15), // "QVector<qint32>"
QT_MOC_LITERAL(34, 419, 8), // "position"
QT_MOC_LITERAL(35, 428, 18), // "documentEditSymbol"
QT_MOC_LITERAL(36, 447, 15), // "QTextCharFormat"
QT_MOC_LITERAL(37, 463, 6), // "format"
QT_MOC_LITERAL(38, 470, 17), // "documentEditBlock"
QT_MOC_LITERAL(39, 488, 11), // "TextBlockID"
QT_MOC_LITERAL(40, 500, 7), // "blockId"
QT_MOC_LITERAL(41, 508, 16), // "QTextBlockFormat"
QT_MOC_LITERAL(42, 525, 16), // "documentEditList"
QT_MOC_LITERAL(43, 542, 10), // "TextListID"
QT_MOC_LITERAL(44, 553, 6), // "listId"
QT_MOC_LITERAL(45, 560, 15), // "QTextListFormat"
QT_MOC_LITERAL(46, 576, 19), // "handleAccountUpdate"
QT_MOC_LITERAL(47, 596, 19), // "answerAccountUpdate"
QT_MOC_LITERAL(48, 616, 3) // "msg"

    },
    "WorkSpace\0requestAccountUpdate\0\0"
    "QSharedPointer<Client>\0client\0nickname\0"
    "icon\0password\0returnClient\0userDisconnected\0"
    "username\0noEditors\0URI\0documentURI\0"
    "newClient\0clientDisconnection\0clientQuit\0"
    "QSslSocket*\0clientSocket\0isForced\0"
    "socketErr\0QAbstractSocket::SocketError\0"
    "socketError\0readMessage\0dispatchMessage\0"
    "MessageCapsule\0message\0sender\0"
    "documentSave\0documentInsertSymbol\0"
    "Symbol\0symbol\0documentDeleteSymbol\0"
    "QVector<qint32>\0position\0documentEditSymbol\0"
    "QTextCharFormat\0format\0documentEditBlock\0"
    "TextBlockID\0blockId\0QTextBlockFormat\0"
    "documentEditList\0TextListID\0listId\0"
    "QTextListFormat\0handleAccountUpdate\0"
    "answerAccountUpdate\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WorkSpace[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,  104,    2, 0x06 /* Public */,
       8,    1,  113,    2, 0x06 /* Public */,
       9,    1,  116,    2, 0x06 /* Public */,
      11,    1,  119,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,  122,    2, 0x0a /* Public */,
      15,    0,  125,    2, 0x0a /* Public */,
      16,    2,  126,    2, 0x0a /* Public */,
      20,    1,  131,    2, 0x0a /* Public */,
      23,    0,  134,    2, 0x0a /* Public */,
      24,    2,  135,    2, 0x0a /* Public */,
      28,    0,  140,    2, 0x0a /* Public */,
      29,    1,  141,    2, 0x0a /* Public */,
      32,    1,  144,    2, 0x0a /* Public */,
      35,    2,  147,    2, 0x0a /* Public */,
      38,    2,  152,    2, 0x0a /* Public */,
      42,    3,  157,    2, 0x0a /* Public */,
      46,    4,  164,    2, 0x0a /* Public */,
      47,    2,  173,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QImage, QMetaType::QString,    4,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 12,   13,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17, QMetaType::Bool,   18,   19,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25, 0x80000000 | 17,   26,   27,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 30,   31,
    QMetaType::Void, 0x80000000 | 33,   34,
    QMetaType::Void, 0x80000000 | 33, 0x80000000 | 36,   34,   37,
    QMetaType::Void, 0x80000000 | 39, 0x80000000 | 41,   40,   37,
    QMetaType::Void, 0x80000000 | 39, 0x80000000 | 43, 0x80000000 | 45,   40,   44,   37,
    QMetaType::Void, 0x80000000 | 17, QMetaType::QString, QMetaType::QImage, QMetaType::QString,   18,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 25,    4,   48,

       0        // eod
};

void WorkSpace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WorkSpace *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestAccountUpdate((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QImage(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->returnClient((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1]))); break;
        case 2: _t->userDisconnected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->noEditors((*reinterpret_cast< URI(*)>(_a[1]))); break;
        case 4: _t->newClient((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1]))); break;
        case 5: _t->clientDisconnection(); break;
        case 6: _t->clientQuit((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: _t->socketErr((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 8: _t->readMessage(); break;
        case 9: _t->dispatchMessage((*reinterpret_cast< MessageCapsule(*)>(_a[1])),(*reinterpret_cast< QSslSocket*(*)>(_a[2]))); break;
        case 10: _t->documentSave(); break;
        case 11: _t->documentInsertSymbol((*reinterpret_cast< Symbol(*)>(_a[1]))); break;
        case 12: _t->documentDeleteSymbol((*reinterpret_cast< QVector<qint32>(*)>(_a[1]))); break;
        case 13: _t->documentEditSymbol((*reinterpret_cast< QVector<qint32>(*)>(_a[1])),(*reinterpret_cast< QTextCharFormat(*)>(_a[2]))); break;
        case 14: _t->documentEditBlock((*reinterpret_cast< TextBlockID(*)>(_a[1])),(*reinterpret_cast< QTextBlockFormat(*)>(_a[2]))); break;
        case 15: _t->documentEditList((*reinterpret_cast< TextBlockID(*)>(_a[1])),(*reinterpret_cast< TextListID(*)>(_a[2])),(*reinterpret_cast< QTextListFormat(*)>(_a[3]))); break;
        case 16: _t->handleAccountUpdate((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QImage(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 17: _t->answerAccountUpdate((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1])),(*reinterpret_cast< MessageCapsule(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< URI >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Symbol >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<qint32> >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<qint32> >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TextBlockID >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TextBlockID >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TextListID >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WorkSpace::*)(QSharedPointer<Client> , QString , QImage , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WorkSpace::requestAccountUpdate)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WorkSpace::*)(QSharedPointer<Client> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WorkSpace::returnClient)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WorkSpace::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WorkSpace::userDisconnected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WorkSpace::*)(URI );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WorkSpace::noEditors)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WorkSpace::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_WorkSpace.data,
    qt_meta_data_WorkSpace,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WorkSpace::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WorkSpace::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WorkSpace.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WorkSpace::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void WorkSpace::requestAccountUpdate(QSharedPointer<Client> _t1, QString _t2, QImage _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WorkSpace::returnClient(QSharedPointer<Client> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WorkSpace::userDisconnected(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WorkSpace::noEditors(URI _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
