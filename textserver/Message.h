#pragma once

#include <QObject>
#include <string>
#include <qpixmap.h>

#define BUFFLEN 256

// TODO: prototipo per far compilare

enum MessageType
{
LoginRequest,
LoginChallenge,
LoginUnlock,
LoginAccessGranted,
LoginError,
AccountCreate,
AccountUpDate,
AccountConfirmed,
AccountDenied,
LogoutRequest,
LogoutConfirmed,
LogoutDenied,
NewDocument,
OpenDocument,
DocumentOpened,
DocumentError,
CharInsert,
CharDelete,
MoveCursor,
UserNameChange,
UserIconChange,
AddUserPresence,
RemoveUserPresence,
WrongMessageType
};

class Message : public QObject
{
	Q_OBJECT
private:
	MessageType m_type;

public:
	Message(MessageType type);
    ~Message();

	/* getter */
	int getType();
	virtual int getIP();
	virtual int getPort();
	virtual QString getUserName();
	virtual QString getNickname();
	virtual QString getPasswd();
	virtual QPixmap getIcon();
	virtual QString getURI();
	virtual QString getDocName();
};

