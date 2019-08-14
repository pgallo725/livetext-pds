#include "MessageHandler.h"

#include "WorkSpace.h"		// TEMPORARY

#include "LoginMessage.h"
#include "LogoutMessage.h"
#include "AccountMessage.h"
#include "DocumentMessage.h"
#include "TextEditMessage.h"
#include "ErrorMessage.h"

#include <QDataStream>


MessageHandler::MessageHandler(OwnerType t, WorkSpace* w)
	: _scenario(t), workspace(w)
{
}

void MessageHandler::process(std::unique_ptr<Message>&& msg, QTcpSocket* socket)
{
	QDataStream streamOut;
	quint16 typeOfMessage = 0;
	QString msg_str;


	streamOut.setDevice(socket);	/* connect stream with socket */

	switch (msg->getType()) {

	case CharInsert:
	{
		TextEditMessage* insertMsg = dynamic_cast<TextEditMessage*>(msg.get());
		workspace->doc->insert(insertMsg->getSymbol());
		break;
	}

	case CharDelete:
	{
		TextEditMessage* deleteMsg = dynamic_cast<TextEditMessage*>(msg.get());
		workspace->doc->removeAt(deleteMsg->getPosition());
		break;
	}

	case MoveCursor:
		break;

	case UserNameChange:
		break;

	case UserIconChange:
		break;

	case AddUserPresence:
		break;

	case RemoveUserPresence:
		break;

		/* Account messages */
	case AccountUpdate:
	{
		AccountMessage* accntUpdate = dynamic_cast<AccountMessage*>(msg.get());

		if (!workspace->editors.find(socket).value()->isLogged()) {
			typeOfMessage = AccountDenied;
			msg_str = "client not logged";
		}
		else if (!workspace->updateAccount(workspace->editors.find(socket).value()->getUser(), accntUpdate->getUserObj()))
		{
			typeOfMessage = AccountDenied;
			msg_str = "cannot modify this user";
		}
		else {
			typeOfMessage = AccountConfirmed;
			msg_str = "update completed";
		}

		streamOut << typeOfMessage << msg_str;
		break;
	}

	/* Logout messages */
	case LogoutRequest:
	{
		if (!workspace->editors.remove(socket)) {
			typeOfMessage = LogoutDenied;
			msg_str = "Cannot logout if you are already loggedout";
		}
		else {
			typeOfMessage = LogoutConfirmed;
			msg_str = "Logout complete";
			// TODO: need to give client to main thread
		}
		streamOut << typeOfMessage << msg_str;

		if (!workspace->editors.size())
			emit workspace->notWorking(workspace->doc->getURI());
		break;
	}

	default:
		typeOfMessage = MessageTypeError;
		streamOut << typeOfMessage << msg->getType();
		break;
	}
}

MessageHandler::~MessageHandler()
{
}
