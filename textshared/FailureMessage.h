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

	void writeContent(QDataStream& stream) const override;

public:

	~FailureMessage() {};

	void readFrom(QDataStream& stream) override;

	QString getDescription();
};

