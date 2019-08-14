#pragma once

#include "Message.h"
#include "User.h"


class AccountMessage :
	public Message
{
private:

	User m_user;
	QString m_reason;
	int m_userId;

public:

	// Build an empty AccountMessage, to be filled later by reading data from a socket stream
	AccountMessage(MessageType m);

	// Create an AccountCreate or AccountUpdate message, with the new/updated account information
	AccountMessage(MessageType accountCreateOrUpdate, User account);

	// Construct the AccountConfirmed message, with the id chosen by the server for the User
	AccountMessage(MessageType accountConfirmed, int userId);

	// Create the AccountDenied message, with a string describing the error
	AccountMessage(MessageType accountDenied, QString reason);

	~AccountMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getters */
	User& getUserObj();
	int getUserId();
	QString getErrorMessage();
};

