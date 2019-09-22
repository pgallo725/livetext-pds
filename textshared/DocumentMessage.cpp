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

void DocumentCreateMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentCreate << quint32(0) << m_docName;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
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

void DocumentRemoveMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentRemove << quint32(0) << m_docURI;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
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

void DocumentOpenMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentOpen << quint32(0) << m_docURI;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
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

void DocumentDismissedMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentDismissed << quint32(0);

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

void DocumentReadyMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentReady << quint32(0) << m_document;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
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

void DocumentCloseMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
}

void DocumentCloseMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentClose << quint32(0);

	socket->write(buffer);
	socket->flush();
}


/*************** DOCUMENT CLOSED CONFIRMATION MESSAGE ***************/

DocumentExitMessage::DocumentExitMessage()
	: Message(DocumentExit)
{
}

void DocumentExitMessage::readFrom(QDataStream& stream)
{
	// NO CONTENT TO READ
}

void DocumentExitMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentExit << quint32(0);

	socket->write(buffer);
	socket->flush();
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

void DocumentErrorMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << DocumentError << quint32(0) << m_error;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

QString DocumentErrorMessage::getErrorMessage() const
{
	return m_error;
}
