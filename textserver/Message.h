#pragma once

#include <QObject>
#include <string>

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
RemoveUserPresence
};

class Message 
{

private:
	MessageType m_type;

public:
	Message(MessageType type);
	~Message();

	/* getter */
	int getType();
	int getIP();
	int getPort();
	QString getUserName();
	QString getNickname();
	QString getPasswd();
};

