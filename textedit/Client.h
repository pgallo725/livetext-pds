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


#define READYREAD_TIMEOUT 100000

class Client : public QObject
{
	Q_OBJECT

private:

	QSslSocket* socket;
	QString username;
	QString nickname;
	QString password;
	SocketBuffer socketBuffer;
	QImage image;
	bool login;

	enum qint16{
		LoginMessage,
		RegisterMessage,
		OpenFileMessage,
		CreateFileMessage,
		DeleteMessage
	};

signals:

	void connectionEstablished();
	void impossibleToConnect();

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
	void removeFileFailed(QString errorType);
	void openFileCompleted(Document document);
	void openFileFailed(QString error);
	void documentDismissed(URI URI);
	void documentExitSuccess(bool isForced = false);
	void documentExitFailed(QString errorType);
	
	// TextEdit Signals
	void recivedSymbol(Symbol character);
	void removeSymbol(QVector<int> position);
	void formatBlock(TextBlockID blockId, QTextBlockFormat fmt, qint32 editorId);

public:

	Client(QObject* parent = 0);
	~Client();

	void messageHandler(MessageCapsule message);
	MessageCapsule readMessage(QDataStream& stream, qint16 typeOfMessage);

public slots:

	// signals handler
	void serverConnection();
	void readBuffer();
	void serverDisconnection();
	void errorHandler(QAbstractSocket::SocketError socketError);
	void writeOnServer();
	void ready();
	void handleSslErrors(const QList<QSslError>& sslErrors);
	// User connection
	void Login();
	void Register();
	void Logout();
	// Data Exchange
	void sendCursor(qint32 userId, qint32 position);
	void receiveCursor(MessageCapsule message);
	void sendChar(Symbol character);
	void removeChar(QVector<int> position);
	void blockModified(TextBlockID blockId, QTextBlockFormat fmt, qint32 editorId);
	void receiveChar(MessageCapsule message);
	void deleteChar(MessageCapsule message);
	void editBlock(MessageCapsule message);
	// Document handler
	void openDocument(URI URI);
	void createDocument(QString name);
	void deleteDocument(URI URI);
	void forceDocumentClose();
	// Server connection
	void Connect(QString ipAddress, quint16 port);
	void Disconnect();
	// Setter & Getter
	void setUsername(QString username);
	void setPassword(QString password);
	void setLogin(bool flag);
	void setNickname(QString nickname);
	void setImage(QImage image);
	bool getLogin();
	// Account handler
	void newUserPresence(MessageCapsule message);
	void updateUserPresence(MessageCapsule message);
	void sendAccountUpdate(QString nickname, QImage image, QString password);
	void deleteUserPresence(MessageCapsule message);
	void removeFromFile(qint32 myId);
};

