#pragma once

#include "Message.h"

class ErrorMessage : public Message
{

private:

	QString m_error;

public:

	// Build an empty ErrorMessage, which will be filled with data read from the socket stream
	ErrorMessage(MessageType m);

	// Use this to construct ErrorMessage items
	ErrorMessage(MessageType messageError, QString description);

	~ErrorMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getter */
	QString getDescription();

};

