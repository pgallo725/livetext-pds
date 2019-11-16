#pragma once

#include <QObject>
#include <QtNetwork>
#include <QThread>
#include <QSharedPointer>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>

//Include headers to handle server Messages
#include <Message.h>
#include <MessageFactory.h>
#include <AccountMessage.h>
#include <LoginMessage.h>
#include <LogoutMessage.h>
#include <PresenceMessage.h>
#include <DocumentMessage.h>
#include <TextEditMessage.h>
#include <FailureMessage.h>
#include <SocketBuffer.h>

//Include headers for Document DataStructure
#include <User.h>
#include <Symbol.h>
#include <Document.h>

#define READYREAD_TIMEOUT 5000


class Client : public QObject
{
	Q_OBJECT

private:

	QSslSocket* socket;
	SocketBuffer socketBuffer;
	QSharedPointer<QThread> workThread;
	
	QMutex m;
	QSharedPointer<QWaitCondition> wc;
	bool sync;

public:

	Client(QSharedPointer<QWaitCondition> wc, QObject* parent = 0);
	~Client();

	// Generic message reader and handler
	MessageCapsule readMessage(QDataStream& stream);
	void messageHandler(MessageCapsule message);
	void setSync();

public slots:

	// User registration and login/logout
	void Login(QString usr, QString passwd);
	void Register(QString usr, QString passwd, QString nick, QImage img);
	void Logout();

	// Server connection handlers
	void Connect(QString ipAddress, quint16 port);
	void Disconnect();
	void serverDisconnection();
	void handleSslErrors(const QList<QSslError>& sslErrors);
	void readBuffer();

	// Document methods
	void openDocument(URI URI);
	void createDocument(QString name);
	void deleteDocument(URI URI);
	void closeDocument();

	// TextEditor message handlers
	void handleCursor(MessageCapsule message);
	void handleCharInsert(MessageCapsule message);
	void handleCharRemove(MessageCapsule message);
	void handleBulkInsert(MessageCapsule message);
	void handleBulkDelete(MessageCapsule message);
	void handleCharFormat(MessageCapsule message);
	void handleBlockFormat(MessageCapsule message);
	void handleListEdit(MessageCapsule message);

	// Send TextEditor messages to server
	void sendCursor(qint32 userId, qint32 position);
	void sendCharInsert(Symbol character, bool isLast);
	void sendCharRemove(Position position);
	void sendBulkInsert(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);
	void sendBulkDelete(QVector<Position> positions);
	void sendCharFormat(Position position, QTextCharFormat fmt);
	void sendBlockFormat(TextBlockID blockId, QTextBlockFormat fmt);
	void sendListEdit(TextBlockID blockId, TextListID listId, QTextListFormat fmt);

	// Account and Presence operations handlers
	void sendAccountUpdate(QString nickname, QImage image, QString password, bool inEditor);
	void handleAddPresence(MessageCapsule message);
	void handleUpdatePresence(MessageCapsule message);
	void handleRemovePresence(MessageCapsule message);

signals:

	// Connection Signals
	void connectionEstablished();
	void impossibleToConnect(QString errorType);
	void abortConnection();

	// Login, Logout & Register
	void loginSuccess(User user);
	void loginFailed(QString errorType);
	void registrationCompleted(User user);
	void registrationFailed(QString errorType);
	void logoutCompleted();
	void logoutFailed(QString errorType);

	// Document Signals
	void openFileCompleted(Document document);
	void fileOperationFailed(QString errorType);
	void documentDismissed(URI URI);
	void documentExitComplete(bool isForced = false);
	void documentExitFailed(QString errorType);
	
	// TextEdit Signals
	void insertSymbol(Symbol character, bool isLast);
	void removeSymbol(Position position);
	void insertBulk(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);
	void removeBulk(QVector<Position> positions);
	void formatSymbol(Position position, QTextCharFormat fmt);
	void formatBlock(TextBlockID blockId, QTextBlockFormat fmt);
	void listEditBlock(TextBlockID blockId, TextListID listId, QTextListFormat fmt);

	// Presence Signals
	void cursorMoved(qint32 position, qint32 user);
	void newUserPresence(qint32 userId, QString username, QImage image);
	void removeUserPresence(qint32 userId);
	void updateUserPresence(qint32 userId, QString username, QImage image);

	// Account signals
	void accountUpdateComplete(User user);
	void accountUpdateFailed(QString error);

};

