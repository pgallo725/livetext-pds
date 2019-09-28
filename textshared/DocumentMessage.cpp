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

void DocumentCreateMessage::writeContent(QDataStream& stream) const
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

void DocumentRemoveMessage::writeContent(QDataStream& stream) const
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

void DocumentOpenMessage::writeContent(QDataStream& stream) const
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

void DocumentDismissedMessage::writeContent(QDataStream& stream) const
{
	// NO CONTENT TO WRITE
}

void DocumentDismissedMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
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

void DocumentReadyMessage::writeContent(QDataStream& stream) const
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

void DocumentCloseMessage::writeContent(QDataStream& stream) const
{
	// NO CONTENT TO WRITE
}

void DocumentCloseMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
}


/*************** DOCUMENT CLOSED CONFIRMATION MESSAGE ***************/

DocumentExitMessage::DocumentExitMessage()
	: Message(DocumentExit)
{
}

void DocumentExitMessage::writeContent(QDataStream& stream) const
{
	// NO CONTENT TO WRITE
}

void DocumentExitMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
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

void DocumentErrorMessage::writeContent(QDataStream& stream) const
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
