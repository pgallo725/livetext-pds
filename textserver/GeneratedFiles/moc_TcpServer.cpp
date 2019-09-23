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
    QByteArrayData data[45];
    char stringdata0[578];
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
QT_MOC_LITERAL(7, 76, 9), // "saveUsers"
QT_MOC_LITERAL(8, 86, 19), // "newClientConnection"
QT_MOC_LITERAL(9, 106, 19), // "clientDisconnection"
QT_MOC_LITERAL(10, 126, 11), // "readMessage"
QT_MOC_LITERAL(11, 138, 15), // "createWorkspace"
QT_MOC_LITERAL(12, 154, 25), // "QSharedPointer<WorkSpace>"
QT_MOC_LITERAL(13, 180, 24), // "QSharedPointer<Document>"
QT_MOC_LITERAL(14, 205, 8), // "document"
QT_MOC_LITERAL(15, 214, 15), // "deleteWorkspace"
QT_MOC_LITERAL(16, 230, 3), // "URI"
QT_MOC_LITERAL(17, 234, 17), // "serveLoginRequest"
QT_MOC_LITERAL(18, 252, 14), // "MessageCapsule"
QT_MOC_LITERAL(19, 267, 11), // "QSslSocket*"
QT_MOC_LITERAL(20, 279, 6), // "socket"
QT_MOC_LITERAL(21, 286, 8), // "username"
QT_MOC_LITERAL(22, 295, 16), // "authenticateUser"
QT_MOC_LITERAL(23, 312, 12), // "clientSocket"
QT_MOC_LITERAL(24, 325, 5), // "token"
QT_MOC_LITERAL(25, 331, 13), // "createAccount"
QT_MOC_LITERAL(26, 345, 8), // "nickname"
QT_MOC_LITERAL(27, 354, 4), // "icon"
QT_MOC_LITERAL(28, 359, 8), // "password"
QT_MOC_LITERAL(29, 368, 13), // "updateAccount"
QT_MOC_LITERAL(30, 382, 14), // "removeDocument"
QT_MOC_LITERAL(31, 397, 6), // "docUri"
QT_MOC_LITERAL(32, 404, 14), // "createDocument"
QT_MOC_LITERAL(33, 419, 6), // "author"
QT_MOC_LITERAL(34, 426, 7), // "docName"
QT_MOC_LITERAL(35, 434, 12), // "openDocument"
QT_MOC_LITERAL(36, 447, 12), // "logoutClient"
QT_MOC_LITERAL(37, 460, 13), // "receiveClient"
QT_MOC_LITERAL(38, 474, 19), // "restoreUserAvaiable"
QT_MOC_LITERAL(39, 494, 18), // "incomingConnection"
QT_MOC_LITERAL(40, 513, 7), // "qintptr"
QT_MOC_LITERAL(41, 521, 9), // "socketErr"
QT_MOC_LITERAL(42, 531, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(43, 560, 11), // "socketError"
QT_MOC_LITERAL(44, 572, 5) // "ready"

    },
    "TcpServer\0newSocket\0\0handle\0"
    "clientToWorkspace\0QSharedPointer<Client>\0"
    "client\0saveUsers\0newClientConnection\0"
    "clientDisconnection\0readMessage\0"
    "createWorkspace\0QSharedPointer<WorkSpace>\0"
    "QSharedPointer<Document>\0document\0"
    "deleteWorkspace\0URI\0serveLoginRequest\0"
    "MessageCapsule\0QSslSocket*\0socket\0"
    "username\0authenticateUser\0clientSocket\0"
    "token\0createAccount\0nickname\0icon\0"
    "password\0updateAccount\0removeDocument\0"
    "docUri\0createDocument\0author\0docName\0"
    "openDocument\0logoutClient\0receiveClient\0"
    "restoreUserAvaiable\0incomingConnection\0"
    "qintptr\0socketErr\0QAbstractSocket::SocketError\0"
    "socketError\0ready"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  119,    2, 0x06 /* Public */,
       4,    1,  122,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  125,    2, 0x0a /* Public */,
       8,    0,  126,    2, 0x0a /* Public */,
       9,    0,  127,    2, 0x0a /* Public */,
      10,    0,  128,    2, 0x0a /* Public */,
      11,    2,  129,    2, 0x0a /* Public */,
      15,    1,  134,    2, 0x0a /* Public */,
      17,    2,  137,    2, 0x0a /* Public */,
      22,    2,  142,    2, 0x0a /* Public */,
      25,    5,  147,    2, 0x0a /* Public */,
      29,    4,  158,    2, 0x0a /* Public */,
      30,    2,  167,    2, 0x0a /* Public */,
      32,    2,  172,    2, 0x0a /* Public */,
      35,    2,  177,    2, 0x0a /* Public */,
      36,    1,  182,    2, 0x0a /* Public */,
      37,    1,  185,    2, 0x0a /* Public */,
      38,    1,  188,    2, 0x0a /* Public */,
      39,    1,  191,    2, 0x0a /* Public */,
      41,    1,  194,    2, 0x0a /* Public */,
      44,    0,  197,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 12, 0x80000000 | 13, 0x80000000 | 5,   14,    6,
    QMetaType::Void, 0x80000000 | 16,   14,
    0x80000000 | 18, 0x80000000 | 19, QMetaType::QString,   20,   21,
    0x80000000 | 18, 0x80000000 | 19, QMetaType::QByteArray,   23,   24,
    0x80000000 | 18, 0x80000000 | 19, QMetaType::QString, QMetaType::QString, QMetaType::QImage, QMetaType::QString,   23,   21,   26,   27,   28,
    0x80000000 | 18, 0x80000000 | 19, QMetaType::QString, QMetaType::QImage, QMetaType::QString,   23,   26,   27,   28,
    0x80000000 | 18, 0x80000000 | 19, 0x80000000 | 16,    6,   31,
    0x80000000 | 18, 0x80000000 | 19, QMetaType::QString,   33,   34,
    0x80000000 | 18, 0x80000000 | 19, 0x80000000 | 16,   23,   31,
    QMetaType::Void, 0x80000000 | 19,   23,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, 0x80000000 | 40,    3,
    QMetaType::Void, 0x80000000 | 42,   43,
    QMetaType::Void,

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
        case 2: _t->saveUsers(); break;
        case 3: _t->newClientConnection(); break;
        case 4: _t->clientDisconnection(); break;
        case 5: _t->readMessage(); break;
        case 6: { QSharedPointer<WorkSpace> _r = _t->createWorkspace((*reinterpret_cast< QSharedPointer<Document>(*)>(_a[1])),(*reinterpret_cast< QSharedPointer<Client>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QSharedPointer<WorkSpace>*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->deleteWorkspace((*reinterpret_cast< URI(*)>(_a[1]))); break;
        case 8: { MessageCapsule _r = _t->serveLoginRequest((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 9: { MessageCapsule _r = _t->authenticateUser((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 10: { MessageCapsule _r = _t->createAccount((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QImage(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 11: { MessageCapsule _r = _t->updateAccount((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QImage(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 12: { MessageCapsule _r = _t->removeDocument((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< URI(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 13: { MessageCapsule _r = _t->createDocument((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 14: { MessageCapsule _r = _t->openDocument((*reinterpret_cast< QSslSocket*(*)>(_a[1])),(*reinterpret_cast< URI(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->logoutClient((*reinterpret_cast< QSslSocket*(*)>(_a[1]))); break;
        case 16: _t->receiveClient((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1]))); break;
        case 17: _t->restoreUserAvaiable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->incomingConnection((*reinterpret_cast< qintptr(*)>(_a[1]))); break;
        case 19: _t->socketErr((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 20: _t->ready(); break;
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
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSslSocket* >(); break;
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
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< URI >(); break;
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
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< URI >(); break;
            }
            break;
        case 15:
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
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< qintptr >(); break;
            }
            break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
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
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void TcpServer::newSocket(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TcpServer::clientToWorkspace(QSharedPointer<Client> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
