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

void DocumentCreateMessage::readFrom(QDataStream& stream)
{
	stream >> m_docName;
}

void DocumentCreateMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_docName;

	streamOut << (quint16)DocumentCreate << bufferData.size() << bufferData;

	//streamOut << (quint16)DocumentCreate << m_docName;
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

void DocumentRemoveMessage::readFrom(QDataStream& stream)
{
	stream >> m_docURI;
}

void DocumentRemoveMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_docURI;

	streamOut << (quint16)DocumentRemove << bufferData.size() << bufferData;

	//streamOut << (quint16)DocumentRemove << m_docURI;
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

void DocumentOpenMessage::readFrom(QDataStream& stream)
{
	stream >> m_docURI;
}

void DocumentOpenMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_docURI;

	streamOut << (quint16)DocumentOpen << bufferData.size() << bufferData;

	//streamOut << (quint16)DocumentOpen << m_docURI;
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

void DocumentDismissedMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
}

void DocumentDismissedMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)DocumentDismissed << (int)0;
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

void DocumentReadyMessage::readFrom(QDataStream& stream)
{
	stream >> m_document;
}

void DocumentReadyMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_document;

	streamOut << (quint16)DocumentReady << bufferData.size() << bufferData;

	//streamOut << (quint16)DocumentReady << m_document;
}

Document DocumentReadyMessage::getDocument() const
{
	return m_document;
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

void DocumentErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

void DocumentErrorMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_error;

	streamOut << (quint16)DocumentError << bufferData.size() << bufferData;

	//streamOut << (quint16)DocumentError << m_error;
}

QString DocumentErrorMessage::getErrorMessage() const
{
	return m_error;
}
