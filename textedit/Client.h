#pragma once

#include <qobject.h>
#include <qtcpsocket.h>
#include <QtNetwork>
#include <QObject>

#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <csignal>
#include <stdio.h>

#include <string>

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

// File to handle Message with the server
#include <AccountMessage.h>
#include <Message.h>
#include <LoginMessage.h>
#include <LogoutMessage.h>
#include <PresenceMessage.h>
#include <DocumentMessage.h>
#include <MessageFactory.h>
#include <TextEditMessage.h>
#include <SocketBuffer.h>


//File for DataStructure
#include <User.h>
#include <Symbol.h>
#include <Document.h>


#define READYREAD_TIMEOUT 10000

class Client : public QObject
{
	Q_OBJECT

private:

	QSslSocket* socket;
	SocketBuffer socketBuffer;

signals:

	// Connection Signal
	void connectionEstablished();
	void impossibleToConnect();
	void abortConnection();

	// Login, Logout & Register
	void loginSuccess(User user);
	void loginFailed(QString errorType);
	void registrationCompleted(User& user);
	void registrationFailed(QString errorType);
	void logoutCompleted();
	void logoutFailed(QString errorType);

	// Presence Signals
	void cursorMoved(qint32 position, qint32 user);
	void userPresence(qint32 userId, QString username, QImage image);	
	void cancelUserPresence(qint32 userId);
	void accountModified(qint32 userId, QString username, QImage image);

	// Account signals
	void personalAccountModified(User user);
	void accountModificationFail(QString error);
	
	// Document Signals
	void openFileCompleted(Document document);
	void fileOperationFailed(QString errorType);
	void documentDismissed(URI URI);
	void documentExitSuccess(bool isForced = false);
	void documentExitFailed(QString errorType);
	
	// TextEdit Signals
	void recivedSymbol(Symbol character, bool isLast);
	void removeSymbol(QVector<int> position);
	void formatSymbol(QVector<qint32> position, QTextCharFormat fmt);
	void formatBlock(TextBlockID blockId, QTextBlockFormat fmt);
	void listEditBlock(TextBlockID blockId, TextListID listId, QTextListFormat fmt);

	// Generic Signals
	void failureSignal(QString errorType);

public:

	Client(QObject* parent = 0);
	~Client();
	
	// Message handler
	void messageHandler(MessageCapsule message);
	MessageCapsule readMessage(QDataStream& stream);

	// Document handler
	void forceDocumentClose();

	// Server connection
	void Disconnect();

public slots:
	// User connection
	void Login(QString usr, QString passwd);
	void Register(QString usr, QString passwd, QString nick, QImage img);
	void Logout();

	// Server connection
	void Connect(QString ipAddress, quint16 port);

	// Signals handler
	void serverConnection();
	void readBuffer();
	void serverDisconnection();
	void handleSslErrors(const QList<QSslError>& sslErrors);

	// Data Exchange
	void receiveCursor(MessageCapsule message);
	void receiveChar(MessageCapsule message);
	void deleteChar(MessageCapsule message);
	void editChar(MessageCapsule message);
	void editBlock(MessageCapsule message);
	void editList(MessageCapsule message);

	// Local to remote
	void sendCursor(qint32 userId, qint32 position);
	void sendChar(Symbol character, bool isLast);
	void removeChar(QVector<int> position);
	void charModified(QVector<qint32> position, QTextCharFormat fmt);
	void blockModified(TextBlockID blockId, QTextBlockFormat fmt);
	void listModified(TextBlockID blockId, TextListID listId, QTextListFormat fmt);

	// Account handler
	void newUserPresence(MessageCapsule message);
	void updateUserPresence(MessageCapsule message);
	void deleteUserPresence(MessageCapsule message);

	// Local to remote
	void removeFromFile(qint32 myId);
	void sendAccountUpdate(QString nickname, QImage image, QString password);

	// Document handler
	void openDocument(URI URI);
	void createDocument(QString name);
	void deleteDocument(URI URI);
	
};

