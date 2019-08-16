#pragma once

#include "Message.h"
#include "Document.h"


class DocumentMessage : public Message
{
private:

	Document m_doc;
	QString m_docNameOrURI;
	QString m_error;

public:

	// Build an empty DocumentMessage, to be filled later reading data from a socket stream,
	// or a simple DocumentRemoved message which has no payload
	DocumentMessage(MessageType m);

	// Costruct New/Open/RemoveDocument or DocumentError messages specifying either the document's 
	// name in the first case, the URI for Open and Remove, the error reason in the last case
	DocumentMessage(MessageType type, QString payload);

	// Use this to create a DocumentOpened response, containing the Document object
	DocumentMessage(MessageType documentOpened, Document document);

	~DocumentMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getter */
	Document getDocument();
	QString getDocumentURI();
	QString getDocumentName();
	QString getErrorMessage();
};

