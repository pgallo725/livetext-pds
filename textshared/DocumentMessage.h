#pragma once
#ifndef _DOCUMENT_MESSAGE_
#define _DOCUMENT_MESSAGE_

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

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentCreateMessage() {};

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

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentRemoveMessage() {};

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

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentOpenMessage() {};

	URI getDocumentURI() const;
};



class DocumentDismissedMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Creates a DocumentDismissed message, confirmation of a document delete operation
	DocumentDismissedMessage();

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentDismissedMessage() {};

};



class DocumentReadyMessage : public Message
{
	friend MessageFactory;

private:

	Document m_document;

protected:

	DocumentReadyMessage();		// empty constructor

	// Use this to create a DocumentReady response, containing the Document object
	DocumentReadyMessage(Document doc);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentReadyMessage() {};
	
	Document getDocument() const;
};


class DocumentCloseMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Creates a DocumentClose message, to inform the server that the client wants to exit the Workspace
	DocumentCloseMessage();

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentCloseMessage() {};

};


class DocumentExitMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Creates a DocumentExit response
	DocumentExitMessage();

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentExitMessage() {};

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

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~DocumentErrorMessage() {};
	
	QString getErrorMessage() const;
};

#endif // _DOCUMENT_MESSAGE_
