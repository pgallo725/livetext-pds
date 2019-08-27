#include "LiveText.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QString>


LiveText::LiveText(QObject* parent) : QObject(parent)
{
	_landingPage = new LandingPage();
	_textEdit = new TextEdit();
	_client = new Client();


	//LANDINGPAGE - LIVETEXT
	connect(_landingPage, &LandingPage::openEditor, this, &LiveText::openEditor); //Open editor
	connect(_landingPage, &LandingPage::connectToServer, this, &LiveText::connectToServer); //Server connection
	connect(_landingPage, &LandingPage::serverLogin, this, &LiveText::Login); //Login
	connect(_landingPage, &LandingPage::serverRegister, this, &LiveText::Register); //Register
	connect(_landingPage, &LandingPage::serverLogout, this, &LiveText::Logout); //Logout


	//CLIENT - LANDING PAGE
	connect(_client, &Client::connectionEstablished, _landingPage, &LandingPage::connectionEstabilished); //Connection estabilished
	connect(_client, &Client::impossibleToConnect, _landingPage, &LandingPage::impossibleToConnect); //Impossibile to conncet
	
	
	//connect(_client, &Client::logoutCompleted, _landingPage, );
	//connect(_client, &Client::logoutFailed, _landingPage, );

	//CLIENT - LIVETEXT
	connect(_client, &Client::loginFailed, this, &LiveText::loginFailed);
	connect(_client, &Client::registrationFailed, this, &LiveText::registrationFailed);
	connect(_client, &Client::loginSuccess, this, &LiveText::loginSuccess);
	connect(_client, &Client::registrationCompleted, this, &LiveText::registrationSuccess);
	
	//connect(_client, &Client::openFileCompleted, this, );
	//connect(_client, &Client::openFileFailed, this, );
	//connect(_client, &Client::removeFileFailed, ,);
	//connect(_client, &Client::documentDismissed,,);

	//TEXTEDIT - LIVETEXT
	connect(_textEdit, &TextEdit::logout, this, &LiveText::returnToLanding);
	connect(_textEdit, &TextEdit::newCursorPosition, this, &LiveText::sendCursor);
	connect(_textEdit, &TextEdit::accountUpdate, this, &LiveText::sendAccountUpdate);

	//CLIENT - TEXTEDIT
	connect(_client, &Client::cursorMoved, _textEdit, &TextEdit::userCursorPositionChanged);
	connect(_client, &Client::userPresence, _textEdit, &TextEdit::newPresence);
	connect(_client, &Client::accountModified, _textEdit, &TextEdit::newPresence);
	connect(_client, &Client::cancelUserPresence, _textEdit, &TextEdit::removePresence);

	//TEXTEDIT - CLIENT

}

LiveText::~LiveText()
{
	delete _landingPage;
	delete _textEdit;
	delete _client;
}



void LiveText::start()
{
	_landingPage->show();
}


void LiveText::openEditor(int mode, QString path = nullptr)
{

	//Chiude finestra attuale
	_landingPage->close();

	//Dimensione finestra
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(_textEdit);

	//Applica la dimensione al TextEdit e lo mette nella finestra corretta
	_textEdit->resize(availableGeometry.width() * 0.6, (availableGeometry.height() * 2) / 3);
	_textEdit->move((availableGeometry.width() - _textEdit->width()) / 2, (availableGeometry.height() - _textEdit->height()) / 2);

	/*switch (mode) {
	case newfile:
		mw->fileNew(path);
		break;

	case uri:
		mw->load(path);
		break;

	default:
		break;
	};*/

	//Mostra la finestra di mw formata
	_textEdit->show();
}

void LiveText::connectToServer(QString ipAddress, quint16 port)
{
	_client->Connect(ipAddress, port);
}

void LiveText::Login(QString username, QString password)
{
	_client->setUsername(username);
	_client->setPassword(password);

	_client->Login();
}


void LiveText::loginFailed(QString errorType)
{
	_landingPage->incorrectOperation(errorType);
	_client->Disconnect();
}



void LiveText::Register(QString username, QString password, QString nickname, QImage icon)
{
	_client->setUsername(username);
	_client->setPassword(password);
	_client->setNickname(nickname);
	_client->setImage(icon);

	_client->Register();
}

void LiveText::registrationFailed(QString errorType)
{
	_landingPage->incorrectOperation(errorType);
	_client->Disconnect();
}

void LiveText::loginSuccess(User user)
{
	_user = user;
	_textEdit->setUser(&_user);
	_landingPage->openLoggedPage();
}

void LiveText::registrationSuccess(User user)
{
	_user = user;
	_textEdit->setUser(&_user);
	_landingPage->openLoggedPage();
}

void LiveText::Logout()
{
	_client->Disconnect();
}


void LiveText::returnToLanding()
{
	_textEdit->close();
	_landingPage->openLoggedPage();
	_landingPage->show();
}

void LiveText::sendCursor(qint32 pos)
{
	_client->sendCursor(_user.getUserId(), pos);
}

void LiveText::sendAccountUpdate(QString name, QImage image)
{
	_client->sendAccountUpdate(_user.getUserId(), name, image);
}


