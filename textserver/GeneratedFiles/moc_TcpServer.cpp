/****************************************************************************
** Meta object code from reading C++ file 'TcpServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../TcpServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpServer_t {
    QByteArrayData data[48];
    char stringdata0[642];
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
QT_MOC_LITERAL(4, 28, 17), // "clientToWorkspace"
QT_MOC_LITERAL(5, 46, 22), // "QSharedPointer<Client>"
QT_MOC_LITERAL(6, 69, 6), // "client"
QT_MOC_LITERAL(7, 76, 17), // "sendAccountUpdate"
QT_MOC_LITERAL(8, 94, 14), // "MessageCapsule"
QT_MOC_LITERAL(9, 109, 3), // "msg"
QT_MOC_LITERAL(10, 113, 19), // "newClientConnection"
QT_MOC_LITERAL(11, 133, 19), // "clientDisconnection"
QT_MOC_LITERAL(12, 153, 11), // "readMessage"
QT_MOC_LITERAL(13, 165, 15), // "createWorkspace"
QT_MOC_LITERAL(14, 181, 25), // "QSharedPointer<WorkSpace>"
QT_MOC_LITERAL(15, 207, 24), // "QSharedPointer<Document>"
QT_MOC_LITERAL(16, 232, 8), // "document"
QT_MOC_LITERAL(17, 241, 15), // "deleteWorkspace"
QT_MOC_LITERAL(18, 257, 3), // "URI"
QT_MOC_LITERAL(19, 261, 18), // "incomingConnection"
QT_MOC_LITERAL(20, 280, 7), // "qintptr"
QT_MOC_LITERAL(21, 288, 14), // "sslSocketError"
QT_MOC_LITERAL(22, 303, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(23, 332, 11), // "socketError"
QT_MOC_LITERAL(24, 344, 14), // "sslSocketReady"
QT_MOC_LITERAL(25, 359, 17), // "serveLoginRequest"
QT_MOC_LITERAL(26, 377, 11), // "QSslSocket*"
QT_MOC_LITERAL(27, 389, 6), // "socket"
QT_MOC_LITERAL(28, 396, 8), // "username"
QT_MOC_LITERAL(29, 405, 16), // "authenticateUser"
QT_MOC_LITERAL(30, 422, 12), // "clientSocket"
QT_MOC_LITERAL(31, 435, 5), // "token"
QT_MOC_LITERAL(32, 441, 13), // "createAccount"
QT_MOC_LITERAL(33, 455, 8), // "nickname"
QT_MOC_LITERAL(34, 464, 4), // "icon"
QT_MOC_LITERAL(35, 469, 8), // "password"
QT_MOC_LITERAL(36, 478, 13), // "updateAccount"
QT_MOC_LITERAL(37, 492, 22), // "workspaceAccountUpdate"
QT_MOC_LITERAL(38, 515, 14), // "removeDocument"
QT_MOC_LITERAL(39, 530, 6), // "docUri"
QT_MOC_LITERAL(40, 537, 14), // "createDocument"
QT_MOC_LITERAL(41, 552, 6), // "author"
QT_MOC_LITERAL(42, 559, 7), // "docName"
QT_MOC_LITERAL(43, 567, 12), // "openDocument"
QT_MOC_LITERAL(44, 580, 14), // "docJustCreated"
QT_MOC_LITERAL(45, 595, 13), // "receiveClient"
QT_MOC_LITERAL(46, 609, 12), // "logoutClient"
QT_MOC_LITERAL(47, 622, 19) // "restoreUserAvaiable"

    },
    "TcpServer\0newSocket\0\0handle\0"
    "clientToWorkspace\0QSharedPointer<Client>\0"
    "client\0sendAccountUpdate\0MessageCapsule\0"
    "msg\0newClientConnection\0clientDisconnection\0"
    "readMessage\0createWorkspace\0"
    "QSharedPointer<WorkSpace>\0"
    "QSharedPointer<Document>\0document\0"
    "deleteWorkspace\0URI\0incomingConnection\0"
    "qintptr\0sslSocketError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "sslSocketReady\0serveLoginRequest\0"
    "QSslSocket*\0socket\0username\0"
    "authenticateUser\0clientSocket\0token\0"
    "createAccount\0nickname\0icon\0password\0"
    "updateAccount\0workspaceAccountUpdate\0"
    "removeDocument\0docUri\0createDocument\0"
    "author\0docName\0openDocument\0docJustCreated\0"
    "receiveClient\0logoutClient\0"
    "restoreUserAvaiable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  129,    2, 0x06 /* Public */,
       4,    1,  132,    2, 0x06 /* Public */,
       7,    2,  135,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,  140,    2, 0x0a /* Public */,
      11,    0,  141,    2, 0x0a /* Public */,
      12,    0,  142,    2, 0x0a /* Public */,
      13,    1,  143,    2, 0x0a /* Public */,
      17,    1,  146,    2, 0x0a /* Public */,
      19,    1,  149,    2, 0x0a /* Public */,
      21,    1,  152,    2, 0x0a /* Public */,
      24,    0,  155,    2, 0x0a /* Public */,
      25,    2,  156,    2, 0x0a /* Public */,
      29,    2,  161,    2, 0x0a /* Public */,
      32,    5,  166,    2, 0x0a /* Public */,
      36,    4,  177,    2, 0x0a /* Public */,
      37,    4,  186,    2, 0x0a /* Public */,
      38,    2,  195,    2, 0x0a /* Public */,
      40,    2,  200,    2, 0x0a /* Public */,
      43,    3,  205,    2, 0x0a /* Public */,
      43,    2,  212,    2, 0x2a /* Public | MethodCloned */,
      45,    1,  217,    2, 0x0a /* Public */,
      46,    1,  220,    2, 0x0a /* Public */,
      47,    1,  223,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 8,    6,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 14, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 18,   16,
    QMetaType::Void, 0x80000000 | 20,    3,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    0x80000000 | 8, 0x80000000 | 26, QMetaType::QString,   27,   28,
    0x80000000 | 8, 0x80000000 | 26, QMetaType::QByteArray,   30,   31,
    0x80000000 | 8, 0x80000000 | 26, QMetaType::QString, QMetaType::QString, QMetaType::QImage, QMetaType::QString,   30,   28,   33,   34,   35,
    0x80000000 | 8, 0x80000000 | 26, QMetaType::QString, QMetaType::QImage, QMetaType::QString,   30,   33,   34,   35,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QString, QMetaType::QImage, QMetaType::QString,    6,   33,   34,   35,
    0x80000000 | 8, 0x80000000 | 26, 0x80000000 | 18,    6,   39,
    0x80000000 | 8, 0x80000000 | 26, QMetaType::QString,   41,   42,
    0x80000000 | 8, 0x80000000 | 26, 0x80000000 | 18, QMetaType::Bool,   30,   39,   44,
    0x80000000 | 8, 0x80000000 | 26, 0x80000000 | 18,   30,   39,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 26,   30,
    QMetaType::Void, QMetaType::QString,   28,

       0        // eod
};

void TcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newSocket((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 1: _t->clientToWorkspace((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1]))); break;
        case 2: _t->sendAccountUpdate((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1])),(*reinterpret_cast< MessageCapsule(*)>(_a[2]))); break;
        case 3: _t->newClientConnection(); break;
        case 4: _t->clientDisconnection(); break;
        case 5: _t->readMessage(); break;
        case 6: { QSharedPointer<WorkSpace> _r = _t->createWorkspace((*reinterpret_cast< QSharedPointer<Document>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QSharedPointer<WorkSpace>*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->deleteWorkspace((*reinterpret_cast< URI(*)>(_a[1]))); break;
        case 8: _t->incomingConnection((*reinterpret_cast< qintptr(*)>(_a[1]))); break;
        case 9: _t->sslSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 10: _t->sslSocketReady(); break;
        case 11: { MessageCapsule _r = _t->serveLoginRequest((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 12: { MessageCapsule _r = _t->authenticateUser((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 13: { MessageCapsule _r = _t->createAccount((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QImage(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 14: { MessageCapsule _r = _t->updateAccount((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QImage(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->workspaceAccountUpdate((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QImage(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 16: { MessageCapsule _r = _t->removeDocument((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< URI(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 17: { MessageCapsule _r = _t->createDocument((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 18: { MessageCapsule _r = _t->openDocument((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< URI(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 19: { MessageCapsule _r = _t->openDocument((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< URI(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->receiveClient((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1]))); break;
        case 21: _t->logoutClient((*reinterpret_cast< QSslSocket*(*)>(_a[1]))); break;
        case 22: _t->restoreUserAvaiable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< URI >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< URI >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< URI >(); break;
            }
            break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< URI >(); break;
            }
            break;
        case 21:
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
            using _t = void (TcpServer::*)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::newSocket)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)(QSharedPointer<Client> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::clientToWorkspace)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TcpServer::*)(QSharedPointer<Client> , MessageCapsule );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpServer::sendAccountUpdate)) {
                *result = 2;
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
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void TcpServer::newSocket(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TcpServer::clientToWorkspace(QSharedPointer<Client> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TcpServer::sendAccountUpdate(QSharedPointer<Client> _t1, MessageCapsule _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
