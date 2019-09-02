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
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentCreate << qint32(0) << m_docName;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
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
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentRemove << qint32(0) << m_docURI;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
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
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentOpen << qint32(0) << m_docURI;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
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
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentDismissed << qint32(0);

	socket->write(buffer);
	socket->flush();
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
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentReady << qint32(0) << m_document;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
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
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentError << qint32(0) << m_error;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
}

QString DocumentErrorMessage::getErrorMessage() const
{
	return m_error;
}
