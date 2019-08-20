/****************************************************************************
** Meta object code from reading C++ file 'TcpServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../TcpServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpServer_t {
    QByteArrayData data[35];
    char stringdata0[426];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpServer_t qt_meta_stringdata_TcpServer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TcpServer"
QT_MOC_LITERAL(1, 10, 9), // "newSocket"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 6), // "handle"
QT_MOC_LITERAL(4, 28, 9), // "saveUsers"
QT_MOC_LITERAL(5, 38, 19), // "newClientConnection"
QT_MOC_LITERAL(6, 58, 19), // "clientDisconnection"
QT_MOC_LITERAL(7, 78, 11), // "readMessage"
QT_MOC_LITERAL(8, 90, 15), // "createWorkspace"
QT_MOC_LITERAL(9, 106, 10), // "WorkSpace*"
QT_MOC_LITERAL(10, 117, 24), // "QSharedPointer<Document>"
QT_MOC_LITERAL(11, 142, 8), // "document"
QT_MOC_LITERAL(12, 151, 22), // "QSharedPointer<Client>"
QT_MOC_LITERAL(13, 174, 6), // "client"
QT_MOC_LITERAL(14, 181, 15), // "deleteWorkspace"
QT_MOC_LITERAL(15, 197, 17), // "serveLoginRequest"
QT_MOC_LITERAL(16, 215, 14), // "MessageCapsule"
QT_MOC_LITERAL(17, 230, 11), // "QTcpSocket*"
QT_MOC_LITERAL(18, 242, 6), // "socket"
QT_MOC_LITERAL(19, 249, 8), // "username"
QT_MOC_LITERAL(20, 258, 16), // "authenticateUser"
QT_MOC_LITERAL(21, 275, 12), // "clientSocket"
QT_MOC_LITERAL(22, 288, 5), // "token"
QT_MOC_LITERAL(23, 294, 13), // "createAccount"
QT_MOC_LITERAL(24, 308, 5), // "User&"
QT_MOC_LITERAL(25, 314, 7), // "newUser"
QT_MOC_LITERAL(26, 322, 13), // "updateAccount"
QT_MOC_LITERAL(27, 336, 11), // "updatedUser"
QT_MOC_LITERAL(28, 348, 14), // "removeDocument"
QT_MOC_LITERAL(29, 363, 6), // "docUri"
QT_MOC_LITERAL(30, 370, 14), // "createDocument"
QT_MOC_LITERAL(31, 385, 6), // "author"
QT_MOC_LITERAL(32, 392, 7), // "docName"
QT_MOC_LITERAL(33, 400, 12), // "openDocument"
QT_MOC_LITERAL(34, 413, 12) // "logoutClient"

    },
    "TcpServer\0newSocket\0\0handle\0saveUsers\0"
    "newClientConnection\0clientDisconnection\0"
    "readMessage\0createWorkspace\0WorkSpace*\0"
    "QSharedPointer<Document>\0document\0"
    "QSharedPointer<Client>\0client\0"
    "deleteWorkspace\0serveLoginRequest\0"
    "MessageCapsule\0QTcpSocket*\0socket\0"
    "username\0authenticateUser\0clientSocket\0"
    "token\0createAccount\0User&\0newUser\0"
    "updateAccount\0updatedUser\0removeDocument\0"
    "docUri\0createDocument\0author\0docName\0"
    "openDocument\0logoutClient"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   92,    2, 0x0a /* Public */,
       5,    0,   93,    2, 0x0a /* Public */,
       6,    0,   94,    2, 0x0a /* Public */,
       7,    0,   95,    2, 0x0a /* Public */,
       8,    2,   96,    2, 0x0a /* Public */,
      14,    1,  101,    2, 0x0a /* Public */,
      15,    2,  104,    2, 0x0a /* Public */,
      20,    2,  109,    2, 0x0a /* Public */,
      23,    2,  114,    2, 0x0a /* Public */,
      26,    2,  119,    2, 0x0a /* Public */,
      28,    2,  124,    2, 0x0a /* Public */,
      30,    2,  129,    2, 0x0a /* Public */,
      33,    2,  134,    2, 0x0a /* Public */,
      34,    1,  139,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 9, 0x80000000 | 10, 0x80000000 | 12,   11,   13,
    QMetaType::Void, QMetaType::QString,   11,
    0x80000000 | 16, 0x80000000 | 17, QMetaType::QString,   18,   19,
    0x80000000 | 16, 0x80000000 | 17, QMetaType::QString,   21,   22,
    0x80000000 | 16, 0x80000000 | 17, 0x80000000 | 24,   21,   25,
    0x80000000 | 16, 0x80000000 | 17, 0x80000000 | 24,   21,   27,
    0x80000000 | 16, 0x80000000 | 17, QMetaType::QString,   13,   29,
    0x80000000 | 16, 0x80000000 | 17, QMetaType::QString,   31,   32,
    0x80000000 | 16, 0x80000000 | 17, QMetaType::QString,   21,   29,
    0x80000000 | 16, 0x80000000 | 17,   21,

       0        // eod
};

void TcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newSocket((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: _t->saveUsers(); break;
        case 2: _t->newClientConnection(); break;
        case 3: _t->clientDisconnection(); break;
        case 4: _t->readMessage(); break;
        case 5: { WorkSpace* _r = _t->createWorkspace((*reinterpret_cast< QSharedPointer<Document>(*)>(_a[1])),(*reinterpret_cast< QSharedPointer<Client>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< WorkSpace**>(_a[0]) = std::move(_r); }  break;
        case 6: _t->deleteWorkspace((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: { MessageCapsule _r = _t->serveLoginRequest((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 8: { MessageCapsule _r = _t->authenticateUser((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 9: { MessageCapsule _r = _t->createAccount((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< User(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 10: { MessageCapsule _r = _t->updateAccount((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< User(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 11: { MessageCapsule _r = _t->removeDocument((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 12: { MessageCapsule _r = _t->createDocument((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 13: { MessageCapsule _r = _t->openDocument((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 14: { MessageCapsule _r = _t->logoutClient((*reinterpret_cast< QTcpSocket*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
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
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
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
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 14:
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
            using _t = void (TcpServer::*)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::newSocket)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TcpServer::staticMetaObject = { {
    &QTcpServer::staticMetaObject,
    qt_meta_stringdata_TcpServer.data,
    qt_meta_data_TcpServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpServer.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int TcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void TcpServer::newSocket(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
