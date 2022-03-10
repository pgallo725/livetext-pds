#pragma once
#ifndef _FAILURE_MESSAGE_
#define _FAILURE_MESSAGE_

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

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~FailureMessage() {};
	
	QString getDescription() const;
};

#endif
