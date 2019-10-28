/****************************************************************************
** Meta object code from reading C++ file 'WorkSpace.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
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
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WorkSpace_t {
    QByteArrayData data[48];
    char stringdata0[612];
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
QT_MOC_LITERAL(19, 216, 9), // "socketErr"
QT_MOC_LITERAL(20, 226, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(21, 255, 11), // "socketError"
QT_MOC_LITERAL(22, 267, 11), // "readMessage"
QT_MOC_LITERAL(23, 279, 15), // "dispatchMessage"
QT_MOC_LITERAL(24, 295, 14), // "MessageCapsule"
QT_MOC_LITERAL(25, 310, 7), // "message"
QT_MOC_LITERAL(26, 318, 6), // "sender"
QT_MOC_LITERAL(27, 325, 12), // "documentSave"
QT_MOC_LITERAL(28, 338, 20), // "documentInsertSymbol"
QT_MOC_LITERAL(29, 359, 7), // "Symbol&"
QT_MOC_LITERAL(30, 367, 6), // "symbol"
QT_MOC_LITERAL(31, 374, 20), // "documentDeleteSymbol"
QT_MOC_LITERAL(32, 395, 15), // "QVector<qint32>"
QT_MOC_LITERAL(33, 411, 8), // "position"
QT_MOC_LITERAL(34, 420, 18), // "documentEditSymbol"
QT_MOC_LITERAL(35, 439, 15), // "QTextCharFormat"
QT_MOC_LITERAL(36, 455, 6), // "format"
QT_MOC_LITERAL(37, 462, 17), // "documentEditBlock"
QT_MOC_LITERAL(38, 480, 11), // "TextBlockID"
QT_MOC_LITERAL(39, 492, 7), // "blockId"
QT_MOC_LITERAL(40, 500, 16), // "QTextBlockFormat"
QT_MOC_LITERAL(41, 517, 16), // "documentEditList"
QT_MOC_LITERAL(42, 534, 10), // "TextListID"
QT_MOC_LITERAL(43, 545, 6), // "listId"
QT_MOC_LITERAL(44, 552, 15), // "QTextListFormat"
QT_MOC_LITERAL(45, 568, 19), // "handleAccountUpdate"
QT_MOC_LITERAL(46, 588, 19), // "answerAccountUpdate"
QT_MOC_LITERAL(47, 608, 3) // "msg"

    },
    "WorkSpace\0requestAccountUpdate\0\0"
    "QSharedPointer<Client>\0client\0nickname\0"
    "icon\0password\0returnClient\0userDisconnected\0"
    "username\0noEditors\0URI\0documentURI\0"
    "newClient\0clientDisconnection\0clientQuit\0"
    "QSslSocket*\0clientSocket\0socketErr\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "readMessage\0dispatchMessage\0MessageCapsule\0"
    "message\0sender\0documentSave\0"
    "documentInsertSymbol\0Symbol&\0symbol\0"
    "documentDeleteSymbol\0QVector<qint32>\0"
    "position\0documentEditSymbol\0QTextCharFormat\0"
    "format\0documentEditBlock\0TextBlockID\0"
    "blockId\0QTextBlockFormat\0documentEditList\0"
    "TextListID\0listId\0QTextListFormat\0"
    "handleAccountUpdate\0answerAccountUpdate\0"
    "msg"
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
      16,    1,  126,    2, 0x0a /* Public */,
      19,    1,  129,    2, 0x0a /* Public */,
      22,    0,  132,    2, 0x0a /* Public */,
      23,    2,  133,    2, 0x0a /* Public */,
      27,    0,  138,    2, 0x0a /* Public */,
      28,    1,  139,    2, 0x0a /* Public */,
      31,    1,  142,    2, 0x0a /* Public */,
      34,    2,  145,    2, 0x0a /* Public */,
      37,    2,  150,    2, 0x0a /* Public */,
      41,    3,  155,    2, 0x0a /* Public */,
      45,    4,  162,    2, 0x0a /* Public */,
      46,    2,  171,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QImage, QMetaType::QString,    4,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 12,   13,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24, 0x80000000 | 17,   25,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void, 0x80000000 | 32,   33,
    QMetaType::Void, 0x80000000 | 32, 0x80000000 | 35,   33,   36,
    QMetaType::Void, 0x80000000 | 38, 0x80000000 | 40,   39,   36,
    QMetaType::Void, 0x80000000 | 38, 0x80000000 | 42, 0x80000000 | 44,   39,   43,   36,
    QMetaType::Void, 0x80000000 | 17, QMetaType::QString, QMetaType::QImage, QMetaType::QString,   18,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 24,    4,   47,

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
        case 6: _t->clientQuit((*reinterpret_cast< QSslSocket*(*)>(_a[1]))); break;
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
