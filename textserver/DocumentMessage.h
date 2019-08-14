#pragma once

#include "Message.h"
#include "Document.h"


class DocumentMessage : public Message
{
private:

	Document m_doc;
	QString m_docURI;
	QString m_docName;
	QString m_username;
	QString m_error;

public:

	// Build an empty DocumentMessage, to be filled later reading data from a socket stream
	DocumentMessage(MessageType m);

	// Create a NewDocument message, specifying the document name and the author
	DocumentMessage(MessageType newDocument, QString docName, QString author);

	// Costruct OpenDocument/RemoveDocument or DocumentError messages specifying
	// the target document's URI in the first 2 cases and the error reason in the last case
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
	QString getAuthorName();
	QString getErrorMessage();
};

