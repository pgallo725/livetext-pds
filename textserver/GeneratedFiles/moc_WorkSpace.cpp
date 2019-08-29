/****************************************************************************
** Meta object code from reading C++ file 'WorkSpace.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../WorkSpace.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WorkSpace.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WorkSpace_t {
    QByteArrayData data[28];
    char stringdata0[331];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WorkSpace_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WorkSpace_t qt_meta_stringdata_WorkSpace = {
    {
QT_MOC_LITERAL(0, 0, 9), // "WorkSpace"
QT_MOC_LITERAL(1, 10, 9), // "noEditors"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 3), // "URI"
QT_MOC_LITERAL(4, 25, 11), // "documentURI"
QT_MOC_LITERAL(5, 37, 12), // "returnClient"
QT_MOC_LITERAL(6, 50, 22), // "QSharedPointer<Client>"
QT_MOC_LITERAL(7, 73, 6), // "client"
QT_MOC_LITERAL(8, 80, 9), // "newClient"
QT_MOC_LITERAL(9, 90, 19), // "clientDisconnection"
QT_MOC_LITERAL(10, 110, 11), // "readMessage"
QT_MOC_LITERAL(11, 122, 12), // "documentSave"
QT_MOC_LITERAL(12, 135, 20), // "documentInsertSymbol"
QT_MOC_LITERAL(13, 156, 7), // "Symbol&"
QT_MOC_LITERAL(14, 164, 6), // "symbol"
QT_MOC_LITERAL(15, 171, 20), // "documentDeleteSymbol"
QT_MOC_LITERAL(16, 192, 15), // "QVector<qint32>"
QT_MOC_LITERAL(17, 208, 8), // "position"
QT_MOC_LITERAL(18, 217, 15), // "dispatchMessage"
QT_MOC_LITERAL(19, 233, 14), // "MessageCapsule"
QT_MOC_LITERAL(20, 248, 7), // "message"
QT_MOC_LITERAL(21, 256, 11), // "QTcpSocket*"
QT_MOC_LITERAL(22, 268, 6), // "sender"
QT_MOC_LITERAL(23, 275, 13), // "updateAccount"
QT_MOC_LITERAL(24, 289, 12), // "clientSocket"
QT_MOC_LITERAL(25, 302, 5), // "User&"
QT_MOC_LITERAL(26, 308, 11), // "updatedUser"
QT_MOC_LITERAL(27, 320, 10) // "clientQuit"

    },
    "WorkSpace\0noEditors\0\0URI\0documentURI\0"
    "returnClient\0QSharedPointer<Client>\0"
    "client\0newClient\0clientDisconnection\0"
    "readMessage\0documentSave\0documentInsertSymbol\0"
    "Symbol&\0symbol\0documentDeleteSymbol\0"
    "QVector<qint32>\0position\0dispatchMessage\0"
    "MessageCapsule\0message\0QTcpSocket*\0"
    "sender\0updateAccount\0clientSocket\0"
    "User&\0updatedUser\0clientQuit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WorkSpace[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   75,    2, 0x0a /* Public */,
       9,    0,   78,    2, 0x0a /* Public */,
      10,    0,   79,    2, 0x0a /* Public */,
      11,    0,   80,    2, 0x0a /* Public */,
      12,    1,   81,    2, 0x0a /* Public */,
      15,    1,   84,    2, 0x0a /* Public */,
      18,    2,   87,    2, 0x0a /* Public */,
      23,    2,   92,    2, 0x0a /* Public */,
      27,    1,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19, 0x80000000 | 21,   20,   22,
    0x80000000 | 19, 0x80000000 | 21, 0x80000000 | 25,   24,   26,
    QMetaType::Void, 0x80000000 | 21,   24,

       0        // eod
};

void WorkSpace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WorkSpace *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->noEditors((*reinterpret_cast< URI(*)>(_a[1]))); break;
        case 1: _t->returnClient((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1]))); break;
        case 2: _t->newClient((*reinterpret_cast< QSharedPointer<Client>(*)>(_a[1]))); break;
        case 3: _t->clientDisconnection(); break;
        case 4: _t->readMessage(); break;
        case 5: _t->documentSave(); break;
        case 6: _t->documentInsertSymbol((*reinterpret_cast< Symbol(*)>(_a[1]))); break;
        case 7: _t->documentDeleteSymbol((*reinterpret_cast< QVector<qint32>(*)>(_a[1]))); break;
        case 8: _t->dispatchMessage((*reinterpret_cast< MessageCapsule(*)>(_a[1])),(*reinterpret_cast< QTcpSocket*(*)>(_a[2]))); break;
        case 9: { MessageCapsule _r = _t->updateAccount((*reinterpret_cast< QTcpSocket*(*)>(_a[1])),(*reinterpret_cast< User(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< MessageCapsule*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->clientQuit((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<qint32> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
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
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WorkSpace::*)(URI );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WorkSpace::noEditors)) {
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
void WorkSpace::noEditors(URI _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WorkSpace::returnClient(QSharedPointer<Client> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
