#pragma once

#include "Message.h"
#include "Document.h"


class DocumentCreateMessage : public Message
{
	friend MessageFactory;

private:

	QString m_docName;

protected:

	DocumentCreateMessage();		// empty constructor

	// Costruct DocumentCreate message specifying the document's name
	DocumentCreateMessage(QString documentName);

public:

	~DocumentCreateMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	QString getDocumentName() const;
};



class DocumentRemoveMessage : public Message
{
	friend MessageFactory;

private:

	URI m_docURI;

protected:

	DocumentRemoveMessage();	 // empty constructor

	// Costruct DocumentRemove message with the URI of the document to be removed
	DocumentRemoveMessage(URI documentURI);

public:

	~DocumentRemoveMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	URI getDocumentURI() const;
};



class DocumentOpenMessage : public Message
{
	friend MessageFactory;

private:

	URI m_docURI;

protected:

	DocumentOpenMessage();		// empty constructor

	// Costruct DocumentOpen message specifying the document's URI
	DocumentOpenMessage(URI documentURI);

public:

	~DocumentOpenMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	URI getDocumentURI() const;
};



class DocumentDismissedMessage : public Message
{
	friend MessageFactory;

private:

protected:

	DocumentDismissedMessage();	 // empty constructor

public:

	~DocumentDismissedMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;
};



class DocumentReadyMessage : public Message
{
	friend MessageFactory;

private:

	Document m_document;

protected:

	DocumentReadyMessage();	// empty constructor

	// Use this to create a DocumentReady response, containing the Document object
	DocumentReadyMessage(Document doc);

public:

	~DocumentReadyMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	Document getDocument() const;
};



class DocumentErrorMessage : public Message
{
	friend MessageFactory;

private:

	QString m_error;

protected:

	DocumentErrorMessage();		// empty constructor

	// Costruct DocumentError message with a string describing the error
	DocumentErrorMessage(QString reason);

public:

	~DocumentErrorMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	QString getErrorMessage() const;
};