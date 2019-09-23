#include "LiveText.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QString>


LiveText::LiveText(QObject* parent) : QObject(parent)
{
	qDebug() << this->thread()->currentThreadId();
	_landingPage = new LandingPage();
	_client = new Client();

	_waitingThread = new QThread(parent);
	connect(_waitingThread, &QThread::finished, _waitingThread, &QThread::deleteLater);

	_landingPage->moveToThread(_waitingThread);
	_waitingThread->start();

	//LANDINGPAGE - LIVETEXT
	//connect(_landingPage, &LandingPage::openEditor, this, &LiveText::openEditor); //Open editor
	connect(_landingPage, &LandingPage::connectToServer, this, &LiveText::connectToServer,Qt::QueuedConnection); //Server connection
	connect(_landingPage, &LandingPage::serverLogin, this, &LiveText::Login); //Login
	connect(_landingPage, &LandingPage::serverRegister, this, &LiveText::Register); //Register
	connect(_landingPage, &LandingPage::serverLogout, this, &LiveText::Logout); //Logout
	connect(_landingPage, &LandingPage::removeDocument, this, &LiveText::removeDocument);
	connect(_landingPage, &LandingPage::addDocument, this, &LiveText::addDocument);
	connect(_landingPage, &LandingPage::openDocument, this, &LiveText::openDocument);

	//LANDINGPAGE - CLIENT
	connect(_landingPage, &LandingPage::newDocument, _client, &Client::createDocument);

	//CLIENT - LANDING PAGE
	connect(_client, &Client::connectionEstablished, _landingPage, &LandingPage::connectionEstabilished); //Connection estabilished
	connect(_client, &Client::impossibleToConnect, _landingPage, &LandingPage::impossibleToConnect); //Impossibile to conncet
	connect(_client, &Client::openFileFailed, _landingPage, &LandingPage::incorrectFileOperation);
	connect(_client, &Client::removeFileFailed, _landingPage, &LandingPage::incorrectFileOperation);

	//connect(_client, &Client::logoutCompleted, _landingPage, );
	//connect(_client, &Client::logoutFailed, _landingPage, );

	//CLIENT - LIVETEXT
	connect(_client, &Client::loginFailed, this, &LiveText::loginFailed);
	connect(_client, &Client::registrationFailed, this, &LiveText::registrationFailed);
	connect(_client, &Client::loginSuccess, this, &LiveText::loginSuccess);
	connect(_client, &Client::registrationCompleted, this, &LiveText::registrationSuccess);
	connect(_client, &Client::personalAccountModified, this, &LiveText::accountUpdated);
	connect(_client, &Client::openFileCompleted, this, &LiveText::openDocumentCompleted);
	connect(_client, &Client::documentDismissed, this, &LiveText::dismissDocumentCompleted);
	
}

LiveText::~LiveText()
{
	delete _landingPage;
	delete _client;
}


//Show landing page
void LiveText::start()
{
	_landingPage->show();
}

//Connection
void LiveText::connectToServer(QString ipAddress, quint16 port)
{
	_client->Connect(ipAddress, port);
}

//Login
void LiveText::Login(QString username, QString password)
{
	_client->setUsername(username);
	_client->setPassword(password);

	_client->Login();
}

void LiveText::loginSuccess(User user)
{
	_user = user;
	_landingPage->setupFileList(_user.getDocuments());
	_landingPage->openLoggedPage();
}

void LiveText::loginFailed(QString errorType)
{
	_landingPage->incorrectOperation(errorType);
	_client->Disconnect();
}

//Registration
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

void LiveText::registrationSuccess(User user)
{
	_user = user;
	_textEdit->setUser(&_user);
	_landingPage->setupFileList(_user.getDocuments());
	_landingPage->openLoggedPage();
}

//Logout
void LiveText::Logout()
{
	_client->Disconnect();
}

//Delete document from list
void LiveText::removeDocument(int index)
{
	_client->deleteDocument(_user.getURIat(index));
}

void LiveText::dismissDocumentCompleted(URI URI)
{
	_user.removeDocument(URI);
	_landingPage->setupFileList(_user.getDocuments());
	_landingPage->documentDismissed();

}

//Open a new document
void LiveText::openDocument(int index)
{
	_client->openDocument(_user.getURIat(index));
}

void LiveText::addDocument(QString uri)
{
	_client->openDocument(URI(uri));
}


void LiveText::openDocumentCompleted(Document doc)
{
	_textEdit = new TextEdit();
	_docEditor = new DocumentEditor(doc, _textEdit, _user);

	//TEXTEDIT - LIVETEXT
	connect(_textEdit, &TextEdit::closeDocument, this, &LiveText::returnToLanding);
	connect(_textEdit, &TextEdit::newCursorPosition, this, &LiveText::sendCursor);
	connect(_textEdit, &TextEdit::accountUpdate, this, &LiveText::sendAccountUpdate);

	//CLIENT - TEXTEDIT
	connect(_client, &Client::cursorMoved, _textEdit, &TextEdit::userCursorPositionChanged); //Received Cursor position
	connect(_client, &Client::userPresence, _textEdit, &TextEdit::newPresence);	//Add/Edit Presence
	connect(_client, &Client::accountModified, _textEdit, &TextEdit::newPresence);
	connect(_client, &Client::cancelUserPresence, _textEdit, &TextEdit::removePresence); //Remove presence
	connect(_client, &Client::accountModificationFail, _textEdit, &TextEdit::accountUpdateFailed);

	
	
	if (!_user.getDocuments().contains(doc.getURI())) {
		_user.addDocument(doc.getURI());
	}
	
	_textEdit->setUser(&_user);
	_docEditor->openDocument();
	
	
	//DOCUMENTEDITOR - CLIENT
	connect(_docEditor, &DocumentEditor::deleteChar, _client, &Client::removeChar);
	connect(_docEditor, &DocumentEditor::insertChar, _client, &Client::sendChar);

	connect(_client, &Client::recivedSymbol, _docEditor, &DocumentEditor::addSymbol);
	connect(_client, &Client::removeSymbol, _docEditor, &DocumentEditor::removeSymbol);

	//ADD DOCUMENT LOADING INTO EDITOR
	openEditor();
}

//Open editor
void LiveText::openEditor()
{
	//Chiude finestra attuale
	_landingPage->closeAll();

	//Dimensione finestra
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(_textEdit);

	//Applica la dimensione al TextEdit e lo mette nella finestra corretta
	_textEdit->resize(availableGeometry.width() * 0.6, (availableGeometry.height() * 2) / 3);
	_textEdit->move((availableGeometry.width() - _textEdit->width()) / 2, (availableGeometry.height() - _textEdit->height()) / 2);

	_textEdit->show();
}

//Close editor
void LiveText::returnToLanding()
{	
	_textEdit->closeEditor();
	
	_client->removeFromFile(_user.getUserId());
	_landingPage->setupFileList(_user.getDocuments());
	_landingPage->openLoggedPage();
	_landingPage->show();
	
	delete _docEditor;
	delete _textEdit;
}

//Send cursor
void LiveText::sendCursor(qint32 pos)
{
	_client->sendCursor(_user.getUserId(), pos);
}

//Account update
void LiveText::sendAccountUpdate(QString nickname, QImage image, QString password)
{
	if (nickname == _user.getNickname()) {
		nickname = QString();
	}

	if (image == _user.getIcon()) {
		image = QImage();
	}


	_client->sendAccountUpdate(nickname, image, password);
}

void LiveText::accountUpdated(User user)
{
	_user = user;
	_textEdit->accountUpdateSuccessful();
}


