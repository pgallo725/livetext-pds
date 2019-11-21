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
	: _useCase(Context::Workspace)
{
	// Connecting all signals to Workspace slots

	connect(this, &MessageHandler::accountUpdate, w, &WorkSpace::handleAccountUpdate, Qt::DirectConnection);

	connect(this, &MessageHandler::charsInsert, w, &WorkSpace::documentInsertSymbols, Qt::DirectConnection);
	connect(this, &MessageHandler::charsDelete, w, &WorkSpace::documentDeleteSymbols, Qt::DirectConnection);
	connect(this, &MessageHandler::charsFormat, w, &WorkSpace::documentEditSymbols, Qt::DirectConnection);
	connect(this, &MessageHandler::blockEdit, w, &WorkSpace::documentEditBlock, Qt::DirectConnection);
	connect(this, &MessageHandler::listEdit, w, &WorkSpace::documentEditList, Qt::DirectConnection);
	connect(this, &MessageHandler::messageDispatch, w, &WorkSpace::dispatchMessage, Qt::DirectConnection);

	connect(this, &MessageHandler::documentClose, w, &WorkSpace::clientQuit, Qt::DirectConnection);
}

MessageHandler::MessageHandler(TcpServer* s)
	: _useCase(Context::Server)
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
		response->send(socket);
		break;
	}

	case LoginUnlock:
	{
		LoginUnlockMessage* loginUnlck = dynamic_cast<LoginUnlockMessage*>(message.get());
		MessageCapsule response = emit loginUnlock(socket, loginUnlck->getToken());
		response->send(socket);
		break;
	}

		/* ACCOUNT MESSAGES */

	case AccountCreate:
	{
		AccountCreateMessage* accntCreate = dynamic_cast<AccountCreateMessage*>(message.get());
		MessageCapsule response = emit accountCreate(socket, accntCreate->getUsername(),
			accntCreate->getNickname(), accntCreate->getIcon(), accntCreate->getPassword());
		response->send(socket);
		break;
	}

	case AccountUpdate:
	{
		if (_useCase == Server) {
			AccountUpdateMessage* accntUpdate = dynamic_cast<AccountUpdateMessage*>(message.get());
			MessageCapsule response = emit accountUpdate(socket, accntUpdate->getNickname(),
				accntUpdate->getIcon(), accntUpdate->getPassword());
			response->send(socket);
		}
		else {
			AccountUpdateMessage* accntUpdate = dynamic_cast<AccountUpdateMessage*>(message.get());
			emit accountUpdate(socket, accntUpdate->getNickname(), accntUpdate->getIcon(),
				accntUpdate->getPassword());
			// the answer is then routed back through separate Workspace slots for success and failure
		}
		break;
	}

		/* DOCUMENT MESSAGES */

	case DocumentCreate:
	{
		DocumentCreateMessage* docMsg = dynamic_cast<DocumentCreateMessage*>(message.get());
		MessageCapsule errorMsg = emit documentCreate(socket, docMsg->getDocumentName());
		if (errorMsg)
			errorMsg->send(socket);
		break;
	}

	case DocumentOpen:
	{
		DocumentOpenMessage* docMsg = dynamic_cast<DocumentOpenMessage*>(message.get());
		MessageCapsule errorMsg = emit documentOpen(socket, docMsg->getDocumentURI());
		if (errorMsg) 
			errorMsg->send(socket);
		break;
	}

	case DocumentRemove:
	{
		DocumentRemoveMessage* docMsg = dynamic_cast<DocumentRemoveMessage*>(message.get());
		MessageCapsule response = emit documentRemove(socket, docMsg->getDocumentURI());
		response->send(socket);
		break;
	}

	case DocumentClose:
	{
		emit documentClose(socket);
		break;
	}

		/* TEXTEDIT MESSAGES */

	case CharsInsert:
	{
		CharsInsertMessage* blkInsertMsg = dynamic_cast<CharsInsertMessage*>(message.get());
		emit charsInsert(blkInsertMsg->getSymbols(), blkInsertMsg->getBlockId(), blkInsertMsg->getBlockFormat());
		emit messageDispatch(message, socket);
		break;
	}

	case CharsDelete:
	{
		CharsDeleteMessage* blkDeleteMsg = dynamic_cast<CharsDeleteMessage*>(message.get());
		emit charsDelete(blkDeleteMsg->getPositions());
		emit messageDispatch(message, socket);
		break;
	}

	case CharsFormat:
	{
		CharsFormatMessage* formatMsg = dynamic_cast<CharsFormatMessage*>(message.get());
		emit charsFormat(formatMsg->getPositions(), formatMsg->getCharFormats());

		// We want to achieve a server-enforced global ordering of format changes, therefore
		// the CharsFormat message is sent back to all editors by not specifying a sender [nullptr]
		emit messageDispatch(message, nullptr);
		break;
	}

	case BlockEdit:
	{
		BlockEditMessage* blockEditMsg = dynamic_cast<BlockEditMessage*>(message.get());
		emit blockEdit(blockEditMsg->getBlockId(), blockEditMsg->getBlockFormat());

		// We want to achieve a server-enforced global ordering of format changes, therefore
		// the BlockEdit message is sent back to all editors by not specifying a sender [nullptr]
		emit messageDispatch(message, nullptr);
		break;
	}

	case ListEdit:
	{
		ListEditMessage* listEditMsg = dynamic_cast<ListEditMessage*>(message.get());
		emit listEdit(listEditMsg->getBlockId(), listEditMsg->getListId(), listEditMsg->getListFormat());

		// We want to achieve a server-enforced global ordering of format changes, therefore
		// the ListEdit message is sent back to all editors by not specifying a sender [nullptr]
		emit messageDispatch(message, nullptr);
		break;
	}

		/* PRESENCE MESSAGES */

	case CursorMove:
	{
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
