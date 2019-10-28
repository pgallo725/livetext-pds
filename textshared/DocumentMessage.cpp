#include "DocumentMessage.h"


/*************** NEW DOCUMENT MESSAGE ***************/

DocumentCreateMessage::DocumentCreateMessage()
	: Message(DocumentCreate)
{
}

DocumentCreateMessage::DocumentCreateMessage(QString documentName)
	: Message(DocumentCreate), m_docName(documentName)
{
}

void DocumentCreateMessage::writeTo(QDataStream& stream) const
{
	stream << m_docName;
}

void DocumentCreateMessage::readFrom(QDataStream& stream)
{
	stream >> m_docName;
}

QString DocumentCreateMessage::getDocumentName() const
{
	return m_docName;
}


/*************** REMOVE DOCUMENT MESSAGE ***************/

DocumentRemoveMessage::DocumentRemoveMessage()
	: Message(DocumentRemove)
{
}

DocumentRemoveMessage::DocumentRemoveMessage(URI documentURI)
	: Message(DocumentRemove), m_docURI(documentURI)
{
}

void DocumentRemoveMessage::writeTo(QDataStream& stream) const
{
	stream << m_docURI;
}

void DocumentRemoveMessage::readFrom(QDataStream& stream)
{
	stream >> m_docURI;
}

URI DocumentRemoveMessage::getDocumentURI() const
{
	return m_docURI;
}


/*************** OPEN DOCUMENT MESSAGE ***************/

DocumentOpenMessage::DocumentOpenMessage()
	: Message(DocumentOpen)
{
}

DocumentOpenMessage::DocumentOpenMessage(URI documentURI)
	: Message(DocumentOpen), m_docURI(documentURI)
{
}

void DocumentOpenMessage::writeTo(QDataStream& stream) const
{
	stream << m_docURI;
}

void DocumentOpenMessage::readFrom(QDataStream& stream)
{
	stream >> m_docURI;
}

URI DocumentOpenMessage::getDocumentURI() const
{
	return m_docURI;
}


/*************** DOCUMENT REMOVED MESSAGE ***************/

DocumentDismissedMessage::DocumentDismissedMessage()
	: Message(DocumentDismissed)
{
}

void DocumentDismissedMessage::writeTo(QDataStream& stream) const
{
	// NO CONTENT TO WRITE
	(void) stream;
}

void DocumentDismissedMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
	(void) stream;
}


/*************** DOCUMENT OPENED MESSAGE ***************/

DocumentReadyMessage::DocumentReadyMessage()
	: Message(DocumentReady)
{
}

DocumentReadyMessage::DocumentReadyMessage(Document doc)
	: Message(DocumentReady), m_document(doc)
{
}

void DocumentReadyMessage::writeTo(QDataStream& stream) const
{
	stream << m_document;
}

void DocumentReadyMessage::readFrom(QDataStream& stream)
{
	stream >> m_document;
}

Document DocumentReadyMessage::getDocument() const
{
	return m_document;
}


/*************** DOCUMENT CLOSE MESSAGE ***************/

DocumentCloseMessage::DocumentCloseMessage()
	: Message(DocumentClose)
{
}

void DocumentCloseMessage::writeTo(QDataStream& stream) const
{
	// NO CONTENT TO WRITE
	(void) stream;
}

void DocumentCloseMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
	(void) stream;
}


/*************** DOCUMENT CLOSED CONFIRMATION MESSAGE ***************/

DocumentExitMessage::DocumentExitMessage()
	: Message(DocumentExit)
{
}

void DocumentExitMessage::writeTo(QDataStream& stream) const
{
	// NO CONTENT TO WRITE
	(void) stream;
}

void DocumentExitMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
	(void) stream;
}


/*************** DOCUMENT ERROR MESSAGE ***************/

DocumentErrorMessage::DocumentErrorMessage()
	: Message(DocumentError)
{
}

DocumentErrorMessage::DocumentErrorMessage(QString reason)
	: Message(DocumentError), m_error(reason)
{
}

void DocumentErrorMessage::writeTo(QDataStream& stream) const
{
	stream << m_error;
}

void DocumentErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

QString DocumentErrorMessage::getErrorMessage() const
{
	return m_error;
}
