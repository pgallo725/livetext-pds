#include "DocumentMessage.h"
#include "ServerException.h"


DocumentMessage::DocumentMessage(MessageType m)
	: Message(m)
{
}

DocumentMessage::DocumentMessage(MessageType type, QString payload)
	: Message(type)
{
	if (m_type == DocumentError)
		m_error = payload;
	else // New/Open/Remove Document
		m_docNameOrURI = payload;
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
		case OpenDocument:
		case RemoveDocument:
			stream >> m_docNameOrURI;
			break;

		case DocumentOpened:
			stream >> m_doc;
			break;

		case DocumentRemoved:
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
		case OpenDocument:
		case RemoveDocument:
			streamOut << m_docNameOrURI;
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
	else return m_docNameOrURI;
}

QString DocumentMessage::getDocumentURI()
{
	if (m_type == DocumentOpened)
		return m_doc.getURI();
	else return m_docNameOrURI;
}

QString DocumentMessage::getErrorMessage()
{
	return m_error;
}
