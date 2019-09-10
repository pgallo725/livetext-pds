#include "MessageHandler.h"

#include "TcpServer.h"
#include "WorkSpace.h"

#include <LoginMessage.h>
#include <LogoutMessage.h>
#include <AccountMessage.h>
#include <DocumentMessage.h>
#include <TextEditMessage.h>
#include <PresenceMessage.h>
#include <FailureMessage.h>


MessageHandler::MessageHandler(WorkSpace* w)
	: _usecase(OwnerType::Workspace)
{
	// Connecting all signals to Workspace slots

	connect(this, &MessageHandler::accountUpdate, w, &WorkSpace::updateAccount, Qt::DirectConnection);

	connect(this, &MessageHandler::charInsert, w, &WorkSpace::documentInsertSymbol, Qt::DirectConnection);
	connect(this, &MessageHandler::charDelete, w, &WorkSpace::documentDeleteSymbol, Qt::DirectConnection);
	connect(this, &MessageHandler::messageDispatch, w, &WorkSpace::dispatchMessage, Qt::DirectConnection);

	connect(this, &MessageHandler::removePresence, w, &WorkSpace::clientQuit, Qt::DirectConnection);
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


void MessageHandler::process(MessageCapsule message, QSslSocket* socket)
{

	switch (message->getType())
	{

		/* LOGIN MESSAGES */

	case LoginRequest:
	{
		LoginRequestMessage* loginRqst = dynamic_cast<LoginRequestMessage*>(message.get());
		MessageCapsule response = emit loginRequest(socket, loginRqst->getUsername());
		response->sendTo(socket);
		break;
	}

	case LoginUnlock:
	{
		LoginUnlockMessage* loginUnlck = dynamic_cast<LoginUnlockMessage*>(message.get());
		MessageCapsule response = emit loginUnlock(socket, loginUnlck->getToken());
		response->sendTo(socket);
		break;
	}

		/* ACCOUNT MESSAGES */

	case AccountCreate:
	{
		AccountCreateMessage* accntCreate = dynamic_cast<AccountCreateMessage*>(message.get());
		MessageCapsule response = emit accountCreate(socket, accntCreate->getUsername(),
			accntCreate->getNickname(), accntCreate->getIcon(), accntCreate->getPassword());
		response->sendTo(socket);
		break;
	}

	case AccountUpdate:
	{
		AccountUpdateMessage* accntUpdate = dynamic_cast<AccountUpdateMessage*>(message.get());
		MessageCapsule response = emit accountUpdate(socket, accntUpdate->getNickname(),
			accntUpdate->getIcon(), accntUpdate->getPassword());
		response->sendTo(socket);
		break;
	}

		/* DOCUMENT MESSAGES */

	case DocumentCreate:
	{
		DocumentCreateMessage* docMsg = dynamic_cast<DocumentCreateMessage*>(message.get());
		MessageCapsule errorMsg = emit documentCreate(socket, docMsg->getDocumentName());
		if (errorMsg)
			errorMsg->sendTo(socket);
		break;
	}

	case DocumentOpen:
	{
		DocumentOpenMessage* docMsg = dynamic_cast<DocumentOpenMessage*>(message.get());
		MessageCapsule errorMsg = emit documentOpen(socket, docMsg->getDocumentURI());
		if (errorMsg) 
			errorMsg->sendTo(socket);
		break;
	}

	case DocumentRemove:
	{
		DocumentRemoveMessage* docMsg = dynamic_cast<DocumentRemoveMessage*>(message.get());
		MessageCapsule response = emit documentRemove(socket, docMsg->getDocumentURI());
		response->sendTo(socket);
		break;
	}

		/* TEXTEDIT MESSAGES */

	case CharInsert:
	{
		CharInsertMessage* insertMsg = dynamic_cast<CharInsertMessage*>(message.get());
		emit charInsert(insertMsg->getSymbol());
		emit messageDispatch(message, socket);
		break;
	}

	case CharDelete:
	{
		CharDeleteMessage* deleteMsg = dynamic_cast<CharDeleteMessage*>(message.get());
		emit charDelete(deleteMsg->getPosition());
		emit messageDispatch(message, socket);
		break;
	}

		/* PRESENCE MESSAGES */

	case CursorMove:
	{
		emit messageDispatch(message, socket);
		break;
	}
		
	case PresenceRemove:
	{
		emit removePresence(socket);
		emit messageDispatch(message, socket);
		break;
	}

		/* LOGOUT MESSAGE */

	case Logout:
	{
		emit userLogout(socket);
		break;
	}

	default:		// Wrong message type already addressed in readMessage,
		return;		// no need to handle error again
	}
}

MessageHandler::~MessageHandler()
{
}
