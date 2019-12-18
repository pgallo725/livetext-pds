#include "LiveText.h"

#include <QString>
#include <QApplication>
#include <QMessageBox>

#include "ProfileEditWindow.h"


LiveText::LiveText(QObject* parent) : QObject(parent), editorOpen(false)
{
	_wc = QSharedPointer<QWaitCondition>(new QWaitCondition());

	//Initialize landing page and client
	_landingPage = new LandingPage();
	_client = new Client(_wc);


	/************************ CONNECTS ************************/

	
	//LANDINGPAGE - LIVETEXT
	connect(_landingPage, &LandingPage::openEditProfile, this, &LiveText::openEditProfile);
	connect(_landingPage, &LandingPage::serverLogout, this, &LiveText::logoutClient);

	//LANDINGPAGE - CLIENT
	connect(_landingPage, &LandingPage::connectToServer, _client, &Client::Connect, Qt::QueuedConnection);			// Connect
	connect(_landingPage, &LandingPage::serverLogin, _client, &Client::Login, Qt::QueuedConnection);				// Login
	connect(_landingPage, &LandingPage::serverRegister, _client, &Client::Register, Qt::QueuedConnection);			// Register
	connect(_landingPage, &LandingPage::newDocument, _client, &Client::createDocument, Qt::QueuedConnection);		// Create document
	connect(_landingPage, &LandingPage::removeDocument, _client, &Client::deleteDocument, Qt::QueuedConnection);	// Remove document
	connect(_landingPage, &LandingPage::openDocument, _client, &Client::openDocument, Qt::QueuedConnection);		// Open document

	//CLIENT - LANDINGPAGE
	connect(_client, &Client::connectionEstablished, _landingPage, &LandingPage::connectionEstabilished, Qt::QueuedConnection);	// Connection estabilished
	connect(_client, &Client::impossibleToConnect, _landingPage, &LandingPage::incorrectOperation, Qt::QueuedConnection);		// Impossibile to conncet
	connect(_client, &Client::fileOperationFailed, _landingPage, &LandingPage::incorrectFileOperation, Qt::QueuedConnection);


	//CLIENT - LIVETEXT
	connect(_client, &Client::loginFailed, this, &LiveText::operationFailed, Qt::QueuedConnection);
	connect(_client, &Client::registrationFailed, this, &LiveText::operationFailed, Qt::QueuedConnection);
	connect(_client, &Client::loginSuccess, this, &LiveText::loginSuccess, Qt::QueuedConnection);
	connect(_client, &Client::registrationCompleted, this, &LiveText::loginSuccess, Qt::QueuedConnection);
	connect(_client, &Client::accountUpdateComplete, this, &LiveText::accountUpdated, Qt::QueuedConnection);
	connect(_client, &Client::openFileCompleted, this, &LiveText::openDocumentCompleted, Qt::QueuedConnection);
	connect(_client, &Client::documentForceClose, this, &LiveText::forceCloseDocument, Qt::QueuedConnection);
	connect(_client, &Client::documentExitComplete, this, &LiveText::closeDocumentCompleted, Qt::QueuedConnection);
	connect(_client, &Client::documentDismissed, this, &LiveText::dismissDocumentCompleted, Qt::QueuedConnection);
	
	//LIVETEXT - CLIENT
	connect(this, &LiveText::closeConnection, _client, &Client::Disconnect, Qt::QueuedConnection);		// Disconnect from server
	connect(this, &LiveText::logout, _client, &Client::Logout, Qt::QueuedConnection);					// Logout
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
*	Disconnection from server
*	Force logout from server
*	Error during login/register
*/

void LiveText::loginSuccess(User user)
{
	//Sets logged user
	_user = user;

	connect(_client, &Client::abortConnection, this, &LiveText::showForceLogout, Qt::QueuedConnection);

	//Open logged page in landing page
	_landingPage->LoginSuccessful(&_user);
}

void LiveText::logoutClient()
{
	//Resets user
	_user = User();

	disconnect(_client, &Client::abortConnection, this, &LiveText::showForceLogout);

	// Logout the client
	emit logout();
}

void LiveText::showForceLogout()
{
	disconnect(_client, &Client::abortConnection, this, &LiveText::showForceLogout);

	QWidget* parent = (editorOpen ? (QWidget*)_textEdit : (QWidget*)_landingPage);
	
	//Create and show critical error popup on top of the current top-level application window
	QMessageBox* err = new QMessageBox(QMessageBox::Icon::Critical, QCoreApplication::applicationName(),
		tr("Server network error, you will be disconnected"), QMessageBox::Ok, parent);
	err->setAttribute(Qt::WA_DeleteOnClose, true);
	err->setWindowFlag(Qt::WindowStaysOnTopHint, true);
	err->setWindowFlag(Qt::X11BypassWindowManagerHint, true);

	err->open(this, SLOT(forceLogout()));
}

void LiveText::forceLogout()
{
	//Return to login page
	_landingPage->pushButtonBackClicked();
	_landingPage->incorrectOperation(tr("Server communication error"));
	_landingPage->show();

	if (editorOpen)		//Close editor
		closeEditor();
}

void LiveText::operationFailed(QString errorType)
{
	//Sets error message to landing page
	_landingPage->incorrectOperation(errorType);

	//Disconnect from server
	emit closeConnection();
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
	connect(_textEdit, &TextEdit::charsAdded, _docEditor, &DocumentEditor::addCharsAtIndex);
	connect(_textEdit, &TextEdit::charsDeleted, _docEditor, &DocumentEditor::deleteCharsAtIndex);
	connect(_textEdit, &TextEdit::charsFormatChanged, _docEditor, &DocumentEditor::changeSymbolFormat);
	connect(_textEdit, &TextEdit::generateExtraSelection, _docEditor, &DocumentEditor::generateExtraSelection, Qt::DirectConnection);
	connect(_textEdit, qOverload<int, int, Qt::Alignment>(&TextEdit::blockFormatChanged), _docEditor, &DocumentEditor::changeBlockAlignment);
	connect(_textEdit, qOverload<int, int, qreal, int>(&TextEdit::blockFormatChanged), _docEditor, &DocumentEditor::changeBlockLineHeight);
	connect(_textEdit, qOverload<int, int, QTextBlockFormat>(&TextEdit::blockFormatChanged), _docEditor, &DocumentEditor::changeBlockFormat);
	connect(_textEdit, &TextEdit::toggleList, _docEditor, &DocumentEditor::toggleList);
	connect(_textEdit, &TextEdit::createNewList, _docEditor, &DocumentEditor::createList);
	connect(_textEdit, &TextEdit::assignBlockToList, _docEditor, &DocumentEditor::assignBlockToList);
	connect(_textEdit, &TextEdit::setBlockNoList, _docEditor, &DocumentEditor::removeBlockFromList);

	//TEXTEDIT - LIVETEXT
	connect(_textEdit, &TextEdit::openEditProfile, this, &LiveText::openEditProfile );


	//CLIENT - TEXTEDIT
	connect(_client, &Client::cursorMoved, _textEdit, &TextEdit::userCursorPositionChanged, Qt::QueuedConnection);	//REMOTE: Cursor position received
	connect(_client, &Client::newUserPresence, _textEdit, &TextEdit::newPresence, Qt::QueuedConnection);			// Add/Edit Presence
	connect(_client, &Client::updateUserPresence, _textEdit, &TextEdit::newPresence, Qt::QueuedConnection);
	connect(_client, &Client::removeUserPresence, _textEdit, &TextEdit::removePresence, Qt::QueuedConnection);		// Remove presence
	connect(_client, &Client::documentExitFailed, _textEdit, &TextEdit::closeDocumentError, Qt::QueuedConnection);	// Problem during close document

	//TEXTEDIT - CLIENT
	connect(_textEdit, &TextEdit::newCursorPosition, _client, &Client::sendCursor, Qt::QueuedConnection);
	connect(_textEdit, &TextEdit::closeDocument, _client, &Client::closeDocument, Qt::QueuedConnection);


	//DOCUMENTEDITOR - CLIENT
	connect(_docEditor, &DocumentEditor::charsAdded, _client, &Client::sendCharsInsert, Qt::QueuedConnection);
	connect(_docEditor, &DocumentEditor::charsDeleted, _client, &Client::sendCharsDelete, Qt::QueuedConnection);
	connect(_docEditor, &DocumentEditor::charsFormatChanged, _client, &Client::sendCharsFormat, Qt::QueuedConnection);
	connect(_docEditor, &DocumentEditor::blockFormatChanged, _client, &Client::sendBlockFormat, Qt::QueuedConnection);
	connect(_docEditor, &DocumentEditor::blockListChanged, _client, &Client::sendListEdit, Qt::QueuedConnection);

	//CLIENT - DOCUMENTEDITOR
	connect(_client, &Client::insertSymbols, _docEditor, &DocumentEditor::charsInsert, Qt::QueuedConnection);
	connect(_client, &Client::removeSymbols, _docEditor, &DocumentEditor::charsDelete, Qt::QueuedConnection);
	connect(_client, &Client::formatBlock, _docEditor, &DocumentEditor::applyBlockFormat, Qt::QueuedConnection);
	connect(_client, &Client::formatSymbols, _docEditor, &DocumentEditor::applySymbolFormat, Qt::QueuedConnection);
	connect(_client, &Client::listEditBlock, _docEditor, &DocumentEditor::listEditBlock, Qt::QueuedConnection);


	//If opening document is not present in user data, it updates data
	if (!_user.hasDocument(doc.getURI()))
		_user.addDocument(doc.getURI());

	//Load document in editor
	_docEditor->openDocument();

	// Synchronize with the client thread
	_client->setSync();
	_wc->wakeAll();

	//Opens text editor
	openEditor();
}


void LiveText::forceCloseDocument()
{
	//Show an error popup to the user inside the editor (on top of any other window)
	QMessageBox* err = new QMessageBox(QMessageBox::Icon::Critical, QCoreApplication::applicationName(),
		tr("Server encountered an error, the document will be closed"), QMessageBox::Ok, _textEdit);
	err->setAttribute(Qt::WA_DeleteOnClose, true);
	err->setWindowFlag(Qt::WindowStaysOnTopHint, true);
	err->setWindowFlag(Qt::X11BypassWindowManagerHint, true);

	err->open(this, SLOT(closeDocumentCompleted()));
}

void LiveText::closeDocumentCompleted()
{
	//Keep the user logged in and return to landing page (document selection)
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
	if (editorOpen)
		return;

	//Show maximized
	_textEdit->showMaximized();
	QApplication::processEvents();	//Fully show text editor before reset cursor position

	//Reset cursor position to top
	_textEdit->resetCursorPosition();

	//Close all window in landing page
	_landingPage->closeAll();

	//Checks flag
	editorOpen = true;
}


void LiveText::closeEditor()
{
	if (!editorOpen)
		return;

	//Close editor
	_textEdit->closeEditor();

	//Delete pointers
	_docEditor->deleteLater();
	_textEdit->deleteLater();

	editorOpen = false;
}

/************************ USER ACCOUNT UPDATE ************************/
/*
*	Open edit profile window
*	Update account in editor/landing page
*/

void LiveText::openEditProfile(bool fromEditor)
{
	QWidget* source = dynamic_cast<QWidget*>(sender());

	//Create edit profile window with user info, parented to the caller widget
	ProfileEditWindow* editProfile = new ProfileEditWindow(_user, fromEditor, source);
	editProfile->setAttribute(Qt::WA_DeleteOnClose, true);

	//Connect reset signal to "close" slot on the dialog
	connect(_landingPage, &LandingPage::landingPageReset, editProfile, &QDialog::close);

	//Connect the account update operation signals to the proper slots
	connect(editProfile, &ProfileEditWindow::accountUpdate, _client, &Client::sendAccountUpdate, Qt::QueuedConnection);
	connect(_client, &Client::accountUpdateComplete, editProfile, &ProfileEditWindow::updateSuccessful, Qt::QueuedConnection);
	connect(_client, &Client::accountUpdateFailed, editProfile, &ProfileEditWindow::updateFailed, Qt::QueuedConnection);

	//Show the edit profile window
	editProfile->open();
}

void LiveText::accountUpdated(User user)
{
	_user = user;

	if (editorOpen)	{
		QString name = _user.getNickname().isEmpty() ? _user.getUsername() : _user.getNickname();
		_textEdit->newPresence(_user.getUserId(), name, _user.getIcon());
	}
	if (_landingPage->isVisible())	{
		_landingPage->resetFields();
		_landingPage->updateUserInfo();
	}
}
