#include "MessageHandler.h"

#include "TcpServer.h"
#include "WorkSpace.h"

#include "LoginMessage.h"
#include "LogoutMessage.h"
#include "AccountMessage.h"
#include "DocumentMessage.h"
#include "TextEditMessage.h"
#include "PresenceMessage.h"
#include "ErrorMessage.h"

#include <QDataStream>


MessageHandler::MessageHandler(WorkSpace* w)
	: _usecase(OwnerType::Workspace)
{
	// Connecting all signals to Workspace slots

	connect(this, &MessageHandler::accountUpdate, w, &WorkSpace::updateAccount, Qt::DirectConnection);

	connect(this, &MessageHandler::charInsert, w, &WorkSpace::documentInsertSymbol, Qt::DirectConnection);
	connect(this, &MessageHandler::charDelete, w, &WorkSpace::documentDeleteSymbol, Qt::DirectConnection);
	connect(this, &MessageHandler::messageDispatch, w, &WorkSpace::dispatchMessage, Qt::DirectConnection);

	connect(this, &MessageHandler::userLogout, w, &WorkSpace::logoutClient, Qt::DirectConnection);
}

MessageHandler::MessageHandler(TcpServer* s)
	: _usecase(OwnerType::Server)
{
	// Connecting signals to TcpServer slots

	connect(this, &MessageHandler::loginRequest, s, &TcpServer::serveLoginRequest, Qt::DirectConnection);
	connect(this, &MessageHandler::loginUnlock, s, &TcpServer::authenticateUser, Qt::DirectConnection);

	connect(this, &MessageHandler::accountCreate, s, &TcpServer::createAccount, Qt::DirectConnection);
	connect(this, &MessageHandler::accountUpdate, s, &TcpServer::updateAccount, Qt::DirectConnection);

	connect(this, &MessageHandler::documentCreate, s, &TcpServer::createDocument, Qt::DirectConnection);
	connect(this, &MessageHandler::documentOpen, s, &TcpServer::openDocument, Qt::DirectConnection);
	connect(this, &MessageHandler::documentRemove, s, &TcpServer::removeDocument, Qt::DirectConnection);

	connect(this, &MessageHandler::userLogout, s, &TcpServer::logoutClient, Qt::DirectConnection);
}


void MessageHandler::process(MessageCapsule message, QTcpSocket* socket)
{

	switch (message->getType())
	{

		/* LOGIN MESSAGES */

	case LoginRequest:
	{
		LoginMessage* loginRqst = dynamic_cast<LoginMessage*>(message.get());
		MessageCapsule response = emit loginRequest(socket, loginRqst->getUsername());
		response->sendTo(socket);
		break;
	}

	case LoginUnlock:
	{
		LoginMessage* loginUnlck = dynamic_cast<LoginMessage*>(message.get());
		MessageCapsule response = emit loginUnlock(socket, loginUnlck->getToken());
		response->sendTo(socket);
		break;
	}

		/* ACCOUNT MESSAGES */

	case AccountCreate:
	{
		AccountMessage* accntCreate = dynamic_cast<AccountMessage*>(message.get());
		MessageCapsule response = emit accountCreate(socket, accntCreate->getUserObj());
		response->sendTo(socket);
		break;
	}

	case AccountUpdate:
	{
		AccountMessage* accntUpdate = dynamic_cast<AccountMessage*>(message.get());
		MessageCapsule response = emit accountUpdate(socket, accntUpdate->getUserObj());
		response->sendTo(socket);
		break;
	}

		/* DOCUMENT MESSAGES */

	case NewDocument:
	{
		DocumentMessage* docMsg = dynamic_cast<DocumentMessage*>(message.get());
		MessageCapsule response = emit documentCreate(socket, docMsg->getDocumentName());
		response->sendTo(socket);
		break;
	}

	case OpenDocument:
	{
		DocumentMessage* docMsg = dynamic_cast<DocumentMessage*>(message.get());
		MessageCapsule response = emit documentOpen(socket, docMsg->getDocumentURI());
		response->sendTo(socket);
		break;
	}

	case RemoveDocument:
	{
		DocumentMessage* docMsg = dynamic_cast<DocumentMessage*>(message.get());
		MessageCapsule response = emit documentRemove(socket, docMsg->getDocumentURI());
		response->sendTo(socket);
		break;
	}

		/* TEXTEDIT MESSAGES */

	case CharInsert:
	{
		TextEditMessage* insertMsg = dynamic_cast<TextEditMessage*>(message.get());
		emit charInsert(insertMsg->getSymbol());
		emit messageDispatch(message, socket);
		break;
	}

	case CharDelete:
	{
		TextEditMessage* deleteMsg = dynamic_cast<TextEditMessage*>(message.get());
		emit charDelete(deleteMsg->getPosition());
		emit messageDispatch(message, socket);
		break;
	}

		/* PRESENCE MESSAGES */

	case MoveCursor:
		emit messageDispatch(message, socket);
		break;

		/* LOGOUT MESSAGE */

	case LogoutRequest:
	{
		MessageCapsule response = emit userLogout(socket);
		response->sendTo(socket);
		break;
	}

	default:		// Wrong message type already addressed in readMessage,
		return;		// no need to handle error again
	}
}

MessageHandler::~MessageHandler()
{
}
