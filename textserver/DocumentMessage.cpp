#include "DocumentMessage.h"
#include "ServerException.h"


DocumentMessage::DocumentMessage(MessageType m)
	: Message(m)
{
}

DocumentMessage::DocumentMessage(MessageType newDocument, QString docName, QString author)
	: Message(newDocument), m_docName(docName), m_username(author)
{
}

DocumentMessage::DocumentMessage(MessageType type, QString payload)
	: Message(type)
{
	if (m_type == DocumentError)
		m_error = payload;
	else // OpenDocument or RemoveDocument
		m_docURI = payload;
}

DocumentMessage::DocumentMessage(MessageType documentOpened, Document document)
	: Message(documentOpened), m_doc(document)
{
}

void DocumentMessage::readFrom(QDataStream& stream)
{
	switch (m_type)
	{
		case NewDocument:
			stream >> m_docName >> m_username;
			m_docURI = m_username + "_" + m_docName + "_" + "deadbeef";	/* TODO: random sequence */
			break;

		case OpenDocument:
		case RemoveDocument:
			stream >> m_docURI;
			break;

		case DocumentOpened:
			stream >> m_doc;
			break;

		case DocumentError:
			stream >> m_error;
			break;

		default:
			throw MessageUnexpectedTypeException(m_type);	// ?
			break;
	}
}

void DocumentMessage::sendTo(QTcpSocket* socket)
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type;

	switch (m_type) 
	{
		case NewDocument:
			streamOut >> m_docName >> m_username;

		case OpenDocument:
		case RemoveDocument:
			streamOut << m_docURI;
			break;

		case DocumentOpened:
			streamOut << m_doc;
			break;

		case DocumentError:
			streamOut << m_error;
			break;

		default:
			throw MessageUnexpectedTypeException(m_type);	// ?
			break;
	}
}


Document DocumentMessage::getDocument()
{
	return m_doc;
}

QString DocumentMessage::getDocumentName()
{
	if (m_type == DocumentOpened)
		return m_doc.getName();
	else return m_docName;
}

QString DocumentMessage::getDocumentURI()
{
	if (m_type == DocumentOpened)
		return m_doc.getURI();
	else return m_docURI;
}

QString DocumentMessage::getAuthorName()
{
	return m_username;
}

QString DocumentMessage::getErrorMessage()
{
	return m_error;
}
