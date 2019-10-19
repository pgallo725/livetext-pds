#include "LiveText.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QString>
#include <QMessageBox>


LiveText::LiveText(QObject* parent) : QObject(parent)
{
	_landingPage = new LandingPage();
	_client = new Client();

	//LANDINGPAGE - LIVETEXT
	//connect(_landingPage, &LandingPage::openEditor, this, &LiveText::openEditor); //Open editor
	connect(_landingPage, &LandingPage::connectToServer, this, &LiveText::connectToServer); //Server connection
	connect(_landingPage, &LandingPage::serverLogin, this, &LiveText::Login); //Login
	connect(_landingPage, &LandingPage::serverRegister, this, &LiveText::Register); //Register
	connect(_landingPage, &LandingPage::serverLogout, this, &LiveText::Logout); //Logout
	connect(_landingPage, &LandingPage::removeDocument, this, &LiveText::removeDocument);
	connect(_landingPage, &LandingPage::addDocument, this, &LiveText::addDocument);
	connect(_landingPage, &LandingPage::openDocument, this, &LiveText::openDocument);
	connect(_landingPage, &LandingPage::editProfile, this, &LiveText::openEditProfile);

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
	connect(_client, &Client::loginFailed, this, &LiveText::operationFailed);
	connect(_client, &Client::registrationFailed, this, &LiveText::operationFailed);
	connect(_client, &Client::loginSuccess, this, &LiveText::loginSuccess);
	connect(_client, &Client::registrationCompleted, this, &LiveText::loginSuccess);
	connect(_client, &Client::personalAccountModified, this, &LiveText::accountUpdated);
	connect(_client, &Client::openFileCompleted, this, &LiveText::openDocumentCompleted);
	connect(_client, &Client::documentDismissed, this, &LiveText::dismissDocumentCompleted);
	connect(_client, &Client::documentExitSuccess, this, &LiveText::closeDocumentCompleted);
	connect(_client, &Client::abortConnection, this, &LiveText::forceLogout);
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

void LiveText::operationFailed(QString errorType)
{
	_landingPage->incorrectOperation(errorType);
	_client->Disconnect();
}

//Login
void LiveText::Login(QString username, QString password)
{
	_client->Login(username, password);
}

void LiveText::loginSuccess(User user)
{
	_user = user;
	_landingPage->setUser(&_user);
	_landingPage->openLoggedPage();
}

//Registration
void LiveText::Register(QString username, QString password, QString nickname, QImage icon)
{
	_client->Register(username, password, nickname, icon);
}

//Logout
void LiveText::Logout()
{
	_client->Logout();
}


void LiveText::forceLogout()
{
	if (_editProfile != nullptr) {
		_editProfile->close();

		delete _editProfile;
		_editProfile = nullptr;
	}

	if (_textEdit != nullptr) {
		_textEdit->criticalError(tr("Server not responding, you will be disconnected"));
		_textEdit->closeEditor();

		delete _docEditor;
		delete _textEdit;


		_docEditor = nullptr;
		_textEdit = nullptr;
	}
	else {
		QMessageBox::StandardButton msgbox = QMessageBox::critical(_landingPage, QCoreApplication::applicationName(), tr("Server not responding, you will be disconnected"), QMessageBox::Ok);
	}

	_landingPage->pushButtonBackClicked();
	_landingPage->incorrectOperation(tr("Server not responding"));
	_landingPage->show();

}

//Delete document from list
void LiveText::removeDocument(int index)
{
	_client->deleteDocument(_user.getURIat(index));
}

void LiveText::dismissDocumentCompleted(URI URI)
{
	_user.removeDocument(URI);
	_landingPage->setupFileList();
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
	_landingPage->stopLoadingAnimation();

	_textEdit = new TextEdit();
	_textEdit->setUser(&_user);

	_docEditor = new DocumentEditor(doc, _textEdit, _user);

	//TEXTEDIT - DOCUMENTEDITOR
	connect(_textEdit, &TextEdit::charDeleted, _docEditor, &DocumentEditor::deleteCharAtIndex);
	connect(_textEdit, &TextEdit::charInserted, _docEditor, &DocumentEditor::addCharAtIndex);
	connect(_textEdit, &TextEdit::generateExtraSelection, _docEditor, &DocumentEditor::generateExtraSelection);
	connect(_textEdit, &TextEdit::blockFormatChanged, _docEditor, &DocumentEditor::changeBlockFormat);
	connect(_textEdit, &TextEdit::symbolFormatChanged, _docEditor, &DocumentEditor::changeSymbolFormat);
	connect(_textEdit, &TextEdit::toggleList, _docEditor, &DocumentEditor::toggleList);
	connect(_textEdit, &TextEdit::createNewList, _docEditor, &DocumentEditor::createList);
	connect(_textEdit, &TextEdit::addBlockToList, _docEditor, &DocumentEditor::assignBlockToList);

	//TEXTEDIT - LIVETEXT
	connect(_textEdit, &TextEdit::closeDocument, this, &LiveText::closeDocument);
	connect(_textEdit, &TextEdit::newCursorPosition, this, &LiveText::sendCursor);
	connect(_textEdit, &TextEdit::accountUpdate, this, &LiveText::sendAccountUpdate);
	connect(_textEdit, &TextEdit::openEditProfile, this, &LiveText::openEditProfile);


	//CLIENT - TEXTEDIT
	connect(_client, &Client::cursorMoved, _textEdit, &TextEdit::userCursorPositionChanged); //Received Cursor position
	connect(_client, &Client::userPresence, _textEdit, &TextEdit::newPresence);	//Add/Edit Presence
	connect(_client, &Client::accountModified, _textEdit, &TextEdit::newPresence);
	connect(_client, &Client::cancelUserPresence, _textEdit, &TextEdit::removePresence); //Remove presence
	connect(_client, &Client::documentExitFailed, _textEdit, &TextEdit::closeDocumentError);


	if (!_user.getDocuments().contains(doc.getURI())) {
		_user.addDocument(doc.getURI());
	}

	_docEditor->openDocument();


	//DOCUMENTEDITOR - CLIENT
	connect(_docEditor, &DocumentEditor::deleteChar, _client, &Client::removeChar);
	connect(_docEditor, &DocumentEditor::insertChar, _client, &Client::sendChar);
	connect(_docEditor, &DocumentEditor::blockFormatChanged, _client, &Client::blockModified);
	connect(_docEditor, &DocumentEditor::symbolFormatChanged, _client, &Client::charModified);
	connect(_docEditor, &DocumentEditor::blockListChanged, _client, &Client::listModified);

	connect(_client, &Client::recivedSymbol, _docEditor, &DocumentEditor::addSymbol);
	connect(_client, &Client::removeSymbol, _docEditor, &DocumentEditor::removeSymbol);
	connect(_client, &Client::formatBlock, _docEditor, &DocumentEditor::applyBlockFormat);
	connect(_client, &Client::formatSymbol, _docEditor, &DocumentEditor::applySymbolFormat);
	connect(_client, &Client::listEditBlock, _docEditor, &DocumentEditor::listEditBlock);

	//ADD DOCUMENT LOADING INTO EDITOR
	openEditor();
}

//Open editor
void LiveText::openEditor()
{
	//Chiude finestra attuale
	_landingPage->closeAll();

	/*//Dimensione finestra
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(_textEdit);

	//Applica la dimensione al TextEdit e lo mette nella finestra corretta
	_textEdit->resize(availableGeometry.width() * 0.6, (availableGeometry.height() * 2) / 3);
	_textEdit->move((availableGeometry.width() - _textEdit->width()) / 2, (availableGeometry.height() - _textEdit->height()) / 2);
	*/

	_textEdit->showMaximized();
}

//Close editor
void LiveText::closeDocument()
{
	_client->removeFromFile(_user.getUserId());
}

void LiveText::closeDocumentCompleted(bool isForced)
{
	if (isForced) {
		_textEdit->criticalError(tr("Server encountered an error, the document will be closed"));
	}

	_textEdit->closeEditor();

	_landingPage->openLoggedPage();
	_landingPage->show();

	delete _docEditor;
	delete _textEdit;

	_docEditor = nullptr;
	_textEdit = nullptr;
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
	if (_textEdit != nullptr) {
		_textEdit->newPresence(_user.getUserId(), _user.getUsername(), _user.getIcon());
	}
	if (_landingPage->isVisible()) {
		_landingPage->updateUserInfo();
	}

	_editProfile->updateSuccessful();
	delete _editProfile;
	_editProfile = nullptr;
}

void LiveText::openEditProfile()
{
	_editProfile = new ProfileEditWindow(&_user);

	connect(_editProfile, &ProfileEditWindow::accountUpdate, this, &LiveText::sendAccountUpdate);
	connect(_client, &Client::accountModificationFail, _editProfile, &ProfileEditWindow::updateFailed);

	_editProfile->exec();
}


