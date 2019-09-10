#pragma once

#include "Message.h"


class FailureMessage : public Message
{
	friend MessageFactory;

private:

	QString m_error;

protected:

	FailureMessage();		// empty constructor

	// Construct FailureMessage items with the appropriate error description
	FailureMessage(QString description);

public:

	~FailureMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	QString getDescription();
};

