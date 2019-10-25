#include "LiveText.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QString>
#include <QMessageBox>


LiveText::LiveText(QObject* parent) : QObject(parent), editorOpen(false)
{
	//Initialize landing page and client
	_landingPage = new LandingPage();
	_client = new Client();


	/************************ CONNECTS ************************/

	//LANDINGPAGE - LIVETEXT
	connect(_landingPage, &LandingPage::editProfile, this, &LiveText::openEditProfile);

	//LANDINGPAGE - CLIENT
	connect(_landingPage, &LandingPage::connectToServer, _client, &Client::Connect); //Connect
	connect(_landingPage, &LandingPage::serverLogin, _client, &Client::Login); //Login
	connect(_landingPage, &LandingPage::serverRegister, _client, &Client::Register); //Register
	connect(_landingPage, &LandingPage::serverLogout, _client, &Client::Logout); //Logout
	connect(_landingPage, &LandingPage::newDocument, _client, &Client::createDocument); //Create document
	connect(_landingPage, &LandingPage::removeDocument, _client, &Client::deleteDocument); //Remove document
	connect(_landingPage, &LandingPage::openDocument, _client, &Client::openDocument); //Open document

	//CLIENT - LANDING PAGE
	connect(_client, &Client::connectionEstablished, _landingPage, &LandingPage::connectionEstabilished); //Connection estabilished
	connect(_client, &Client::impossibleToConnect, _landingPage, &LandingPage::impossibleToConnect); //Impossibile to conncet
	connect(_client, &Client::fileOperationFailed, _landingPage, &LandingPage::incorrectFileOperation);


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

//Startup
void LiveText::launch()
{
	_landingPage->show();
}


/************************ SERVER CONNECTION ************************/
/*
*	Connection to server
*	Force logout from server
*	Error during login/register
*/

void LiveText::loginSuccess(User user)
{
	//Sets logged user
	_user = user;

	_editProfile = new ProfileEditWindow(_user);

	connect(_editProfile, &ProfileEditWindow::accountUpdate, _client, &Client::sendAccountUpdate);
	connect(_client, &Client::accountModificationFail, _editProfile, &ProfileEditWindow::updateFailed);

	//Open logged page in landing page
	_landingPage->LoginSuccessful(&_user);
}


void LiveText::operationFailed(QString errorType)
{
	//Sets error message to landing page
	_landingPage->incorrectOperation(errorType);

	//Disconnect from server
	_client->Disconnect();
}

void LiveText::forceLogout()
{
	//Close edit profile (if opened)
	if (_editProfile->isVisible())
		_editProfile->close();

	//Shows landing page again
	_landingPage->pushButtonBackClicked();
	_landingPage->incorrectOperation(tr("Server not responding"));
	_landingPage->show();

	if (editorOpen) {
		//Show an error on editor
		_textEdit->criticalError(tr("Server not responding, you will be disconnected"));
		//Close editor
		closeEditor();
	}
	else
		QMessageBox::StandardButton(QMessageBox::critical(_landingPage, QCoreApplication::applicationName(), tr("Server not responding, you will be disconnected"), QMessageBox::Ok));
}

/************************ DOCUMENT OPERATIONS ************************/
/*
*	Open document
*	Close document
*	Remove document
*/

void LiveText::openDocumentCompleted(Document doc)
{
	//Initialize text editor and local document editor
	_textEdit = new TextEdit(_user);
	_docEditor = new DocumentEditor(doc, _textEdit, _user);


	/********************** CONNECTS **********************/

	//TEXTEDIT - DOCUMENTEDITOR
	connect(_textEdit, &TextEdit::charDeleted, _docEditor, &DocumentEditor::deleteCharAtIndex);
	connect(_textEdit, &TextEdit::charInserted, _docEditor, &DocumentEditor::addCharAtIndex);
	connect(_textEdit, &TextEdit::generateExtraSelection, _docEditor, &DocumentEditor::generateExtraSelection);
	connect(_textEdit, &TextEdit::blockFormatChanged, _docEditor, &DocumentEditor::changeBlockFormat);
	connect(_textEdit, &TextEdit::symbolFormatChanged, _docEditor, &DocumentEditor::changeSymbolFormat);
	connect(_textEdit, &TextEdit::toggleList, _docEditor, &DocumentEditor::toggleList);
	connect(_textEdit, &TextEdit::createNewList, _docEditor, &DocumentEditor::createList);
	connect(_textEdit, &TextEdit::assignBlockToList, _docEditor, &DocumentEditor::assignBlockToList);
	connect(_textEdit, &TextEdit::setBlockNoList, _docEditor, &DocumentEditor::removeBlockFromList);

	//TEXTEDIT - LIVETEXT
	connect(_textEdit, &TextEdit::openEditProfile, this, &LiveText::openEditProfile);


	//CLIENT - TEXTEDIT
	connect(_client, &Client::cursorMoved, _textEdit, &TextEdit::userCursorPositionChanged); //REMOTE: Cursor position recived
	connect(_client, &Client::userPresence, _textEdit, &TextEdit::newPresence);	//Add/Edit Presence
	connect(_client, &Client::accountModified, _textEdit, &TextEdit::newPresence);
	connect(_client, &Client::cancelUserPresence, _textEdit, &TextEdit::removePresence); //Remove presence
	connect(_client, &Client::documentExitFailed, _textEdit, &TextEdit::closeDocumentError); //Problem during close document

	//TEXTEDIT - CLIENT
	connect(_textEdit, &TextEdit::newCursorPosition, _client, &Client::sendCursor);
	connect(_textEdit, &TextEdit::closeDocument, _client, &Client::removeFromFile);

	//DOCUMENTEDITOR - CLIENT
	connect(_docEditor, &DocumentEditor::deleteChar, _client, &Client::removeChar);
	connect(_docEditor, &DocumentEditor::insertChar, _client, &Client::sendChar);
	connect(_docEditor, &DocumentEditor::blockFormatChanged, _client, &Client::blockModified);
	connect(_docEditor, &DocumentEditor::symbolFormatChanged, _client, &Client::charModified);
	connect(_docEditor, &DocumentEditor::blockListChanged, _client, &Client::listModified);


	//CLIENT - DOCUMENTEDITOR
	connect(_client, &Client::recivedSymbol, _docEditor, &DocumentEditor::addSymbol, Qt::QueuedConnection);
	connect(_client, &Client::removeSymbol, _docEditor, &DocumentEditor::removeSymbol, Qt::QueuedConnection);
	connect(_client, &Client::formatBlock, _docEditor, &DocumentEditor::applyBlockFormat, Qt::QueuedConnection);
	connect(_client, &Client::formatSymbol, _docEditor, &DocumentEditor::applySymbolFormat, Qt::QueuedConnection);
	connect(_client, &Client::listEditBlock, _docEditor, &DocumentEditor::listEditBlock, Qt::QueuedConnection);

	//If opening document is not present in user data, it updates data
	if (!_user.getDocuments().contains(doc.getURI())) {
		_user.addDocument(doc.getURI());
	}

	//Load document in editor
	_docEditor->openDocument();

	//Opens text editor
	openEditor();
}


void LiveText::closeDocumentCompleted(bool isForced)
{
	if (isForced) {
		//If it is forced print an error message in editor
		_textEdit->criticalError(tr("Server encountered an error, the document will be closed"));
	}

	//Reopen logged page in landing page
	_landingPage->LoginSuccessful(&_user);
	_landingPage->show();

	closeEditor();
}


void LiveText::dismissDocumentCompleted(URI URI)
{
	//Remove document from uri
	_user.removeDocument(URI);

	//Rebuild document list in landing page
	_landingPage->documentDismissed();
}


/************************ EDITOR OPERATIONS ************************/
/*
*	Open editor
*	Close editor
*/

void LiveText::openEditor()
{
	//Center and resizes window
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(_textEdit);
	_textEdit->resize(availableGeometry.width() * 0.6, (availableGeometry.height() * 2) / 3);
	_textEdit->move((availableGeometry.width() - _textEdit->width()) / 2, (availableGeometry.height() - _textEdit->height()) / 2);

	//Show maximized
	_textEdit->showMaximized();

	//Close all window in landing page
	_landingPage->closeAll();

	//Checks flag
	editorOpen = true;
}


void LiveText::closeEditor()
{
	//Close editor
	_textEdit->closeEditor();

	//Delete pointers
	delete _docEditor;
	delete _textEdit;

	editorOpen = false;
}

/************************ USER ACCOUNT UPDATE ************************/
/*
*	Open edit profile window
*	Update account in editor/landing page
*/


void LiveText::openEditProfile()
{
	_editProfile->updateInfo();
	_editProfile->exec();
}


void LiveText::accountUpdated(User user)
{
	_user = user;

	if (editorOpen) {
		_textEdit->newPresence(_user.getUserId(), _user.getUsername(), _user.getIcon());
	}
	if (_landingPage->isVisible()) {
		_landingPage->updateUserInfo();
	}

	_editProfile->updateSuccessful();
}