#pragma once

#include "Message.h"
#include "User.h"


class LoginMessage :
	public Message
{
private:
	
	QString m_payload;
	User m_user;

public:

	// Build an empty LoginMessage object, which will be filled later
	LoginMessage(MessageType m);

	// Use this to construct LoginRequest, LoginChallenge, LoginUnlock or LoginError messages
	LoginMessage(MessageType loginType, QString payload);

	// Use this constructor for LoginAccessGranted messages
	LoginMessage(MessageType loginGranted, User user);			

	~LoginMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getters */
	QString getUsername();
	QString getNonce();
	QString getToken();
	QString getErrorMessage();
	User getLoggedUser();
};

