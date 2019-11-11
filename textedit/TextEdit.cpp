#include <QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QBitmap>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTextCursor>
#include <QPainter>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QLabel>
#include <QMimeData>
#include <QScrollBar>
#include <QRectF>
#include <QToolButton>
#include <QScrollArea>
#include <QTimer>
#include <QDesktopServices>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>

#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif

#include <QPrinter>
#include <QTimer>
#include <QLayout>
#include <QAbstractTextDocumentLayout>
#include <QAbstractScrollArea>

#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "textedit.h"

const QString rsrcPath = ":/images";


TextEdit::TextEdit(User& user, QWidget* parent) : QMainWindow(parent), _user(user)
{

	//About widget
	_aboutwindow = new AboutWindow();


	/**************************** GUI SETUP ****************************/

	setupMainWindow();
	setupEditorActions();


	/**************************** CONNECTS ****************************/

	//GUI update in case of format change or cursor position changed
	connect(_textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::cursorPositionChanged);


	//Online users cursor redraw in case of window aspect, char format, cursor position changed
	connect(_textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::redrawAllCursors);
	connect(_textEdit->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::redrawAllCursors);
	connect(_textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::redrawAllCursors);

	//Online users text highlight redraw in case of window aspect, char format, cursor position changed
	connect(_textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::updateUsersSelections);

	//Mandatory to intercept character insertion, the document emit this signal every time a character inside document is added/removed
	connect(_textEdit->document(), &QTextDocument::contentsChange, this, &TextEdit::contentsChange);

	//Adapt textEditor layout according to document
	connect(_textEdit->document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged, this, &TextEdit::resizeEditor);



	//If i can use clipboard setup connects from QTextEdit of cut/copy and clipboard management
#ifndef QT_NO_CLIPBOARD
	connect(_textEdit, &QTextEdit::copyAvailable, actionCut, &QAction::setEnabled);
	connect(_textEdit, &QTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);
	connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEdit::clipboardDataChanged);
#endif

	//Setup extra cursor
	_extraCursor = new QTextCursor(_textEdit->document());

	/**************************** EDITOR INITIALIZATION ****************************/

	const QSignalBlocker blocker(_textEdit->document());

	//Set default character and size
	QFont textFont("Helvetica");
	textFont.setStyleHint(QFont::SansSerif);
	textFont.setPointSize(12);
	_textEdit->setFont(textFont);
	_textEdit->document()->setDefaultFont(textFont);

	//Disable undo/redo
	_textEdit->setUndoRedoEnabled(false);

	//Set document margin
	_textEdit->document()->setDocumentMargin(64);

	//Initialize GUI, activating buttons according to current conditions
	fontChanged(_textEdit->font());
	colorChanged(_textEdit->textColor());
	alignmentChanged(_textEdit->alignment());

#ifndef QT_NO_CLIPBOARD
	//Initialize copy/cut/paste actions
	actionCut->setEnabled(false);
	actionCopy->setEnabled(false);
	actionPaste->setEnabled(false);
#endif

	//Setup cursor position
	_currentCursorPosition = -1;

	//Generate new presence for current user
	newPresence(_user.getUserId(), _user.getUsername(), _user.getIcon());
}

/**************************** GUI SETUP ****************************/
/*
*	Setup window appearance (centralWidget/Resize/Borders/Style)
*	This functions configure all Menu entries
*	File (Export PDF, Print)
*	Share document
*	Edit (Cut, Copy, Paste)
*	Text (TextFormat, Lists, Alignment)
*	Account actions (Edit profile, Close document)
*/


void TextEdit::setupMainWindow()
{
	//Setting window title
	setWindowTitle(QCoreApplication::applicationName());
	//Setting window icon
	setWindowIcon(QIcon(rsrcPath + "/misc/logo.png"));

	//Center and resizes window
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);

	int screenWidth = availableGeometry.width() * 0.6;
	resize(screenWidth + 100, availableGeometry.height() * 0.75);
	move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);

	//Inizialize Qt text editor
	_textEdit = new QTextEditWrapper();
	_textEdit->setMaximumWidth(screenWidth);
	_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//Creates 3D effect of document
	_textEdit->setFrameStyle(QFrame::Plain);
	//_textEdit->setFrameShadow(QFrame::Raised);
	_textEdit->setLineWidth(1);

	//Set central widget and layout
	QWidget* cntr = new QWidget(this);

	//Sets white background
	cntr->setAutoFillBackground(true);
	QPalette pal = cntr->palette();
	pal.setColor(QPalette::Window, QColor(128, 128, 128));
	cntr->setPalette(pal);

	//Generate scroll area to set QTextEditor
	area = new QScrollArea(this);
	area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	area->setWidgetResizable(true);
	area->setWidget(cntr);


	//Set layout to QScroll Area
	QHBoxLayout* bl = new QHBoxLayout(area);
	bl->setContentsMargins(0, 24, 0, 0);
	bl->setAlignment(Qt::AlignHCenter); 	//Set widget alignment
	bl->addWidget(_textEdit);	//Add text editor to widget

	//set widget layout
	cntr->setLayout(bl);

	//Set QMainWindow central widget
	setCentralWidget(area);

	//Hide status bar
	statusBar()->hide();

	//Makes keyboard focused on text editor
	_textEdit->setFocus();
}



void TextEdit::setupEditorActions()
{
	/********** DOCUMENT MENU **********/

	//New toolbar
	QToolBar* tb = addToolBar(tr("Document Actions"));

	//New menu
	QMenu* menu = menuBar()->addMenu(tr("&Document"));

	//Share URI, opens a box with URI pasted inside
	const QIcon shareIcon = QIcon(rsrcPath + "/editor/share.png");
	QAction* a = menu->addAction(shareIcon, tr("Share URI"), this, &TextEdit::fileShare);
	tb->addAction(a);

	menu->addSeparator();

	//If the print plugin is enabled
#ifndef QT_NO_PRINTER

	//Export document in PDF
	const QIcon exportPdfIcon = QIcon(rsrcPath + "/editor/exportpdf.png");
	a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(Qt::CTRL + Qt::Key_D);
	tb->addAction(a);

	//Print document
	const QIcon filePrintIcon = QIcon(rsrcPath + "/editor/fileprint.png");
	menu->addAction(filePrintIcon, tr("Print Preview..."), this, &TextEdit::filePrintPreview);

	a = menu->addAction(filePrintIcon, tr("&Print..."), this, &TextEdit::filePrint);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(QKeySequence::Print);
	tb->addAction(a);

#endif
	menu->addSeparator();

	//Close document
	const QIcon closeDocumentIcon(rsrcPath + "/misc/logout.png");
	QAction* closeDocoumentAction = menu->addAction(closeDocumentIcon, tr("&Close Document"), this, &TextEdit::askBeforeCloseDocument);



	/********** EDIT MENU **********/

	tb = addToolBar(tr("Edit Actions"));
	menu = menuBar()->addMenu(tr("&Edit"));


#ifndef QT_NO_CLIPBOARD
	//Cut
	const QIcon cutIcon = QIcon(rsrcPath + "/editor/editcut.png");
	actionCut = menu->addAction(cutIcon, tr("Cu&t"), _textEdit, &QTextEdit::cut);
	actionCut->setPriority(QAction::LowPriority);
	actionCut->setShortcut(QKeySequence::Cut);
	tb->addAction(actionCut);

	//Copy
	const QIcon copyIcon = QIcon(rsrcPath + "/editor/editcopy.png");
	actionCopy = menu->addAction(copyIcon, tr("&Copy"), _textEdit, &QTextEdit::copy);
	actionCopy->setPriority(QAction::LowPriority);
	actionCopy->setShortcut(QKeySequence::Copy);
	tb->addAction(actionCopy);

	//Paste
	const QIcon pasteIcon = QIcon(rsrcPath + "/editor/editpaste.png");
	actionPaste = menu->addAction(pasteIcon, tr("&Paste"), _textEdit, &QTextEdit::paste);
	actionPaste->setPriority(QAction::LowPriority);
	actionPaste->setShortcut(QKeySequence::Paste);
	tb->addAction(actionPaste);

	//Checks if there's some items in the clipboards
	clipboardDataChanged();
#endif

	//Select all
	menu->addSeparator();
	menu->addAction(tr("Select all"), _textEdit, &QTextEdit::selectAll, QKeySequence::SelectAll);



	/********** FORMAT MENU **********/

	tb = addToolBar(tr("Format Actions"));
	QMenu* formatMenu = menuBar()->addMenu(tr("F&ormat"));

	//Bold
	const QIcon boldIcon = QIcon(rsrcPath + "/editor/textbold.png");
	actionTextBold = formatMenu->addAction(boldIcon, tr("&Bold"), this, &TextEdit::textBold);
	actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
	actionTextBold->setPriority(QAction::LowPriority);

	//Checkable
	actionTextBold->setCheckable(true);

	//Sets menu entry style
	QFont bold;
	bold.setBold(true);
	actionTextBold->setFont(bold);

	//Add action to toolbar
	tb->addAction(actionTextBold);


	//Italic
	const QIcon italicIcon = QIcon(rsrcPath + "/editor/textitalic.png");
	actionTextItalic = formatMenu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);
	actionTextItalic->setPriority(QAction::LowPriority);
	actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
	actionTextItalic->setCheckable(true);

	QFont italic;
	italic.setItalic(true);
	actionTextItalic->setFont(italic);

	tb->addAction(actionTextItalic);


	//Underline
	const QIcon underlineIcon = QIcon(rsrcPath + "/editor/textunder.png");
	actionTextUnderline = formatMenu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);
	actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
	actionTextUnderline->setPriority(QAction::LowPriority);
	actionTextUnderline->setCheckable(true);

	QFont underline;
	underline.setUnderline(true);
	actionTextUnderline->setFont(underline);

	tb->addAction(actionTextUnderline);

	//Strikethrough
	const QIcon strikeIcon = QIcon(rsrcPath + "/editor/textstrikethrough.png");
	actionTextStrikeout = formatMenu->addAction(strikeIcon, tr("&Strikeout"), this, &TextEdit::textStrikeout);
	actionTextStrikeout->setPriority(QAction::LowPriority);
	actionTextStrikeout->setCheckable(true);

	QFont strikethrough;
	strikethrough.setStrikeOut(true);
	actionTextStrikeout->setFont(strikethrough);

	tb->addAction(actionTextStrikeout);

	formatMenu->addSeparator();
	tb->addSeparator();

	//Color
	QPixmap pix(rsrcPath + "/editor/textcolor.png");
	actionTextColor = formatMenu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);
	tb->addAction(actionTextColor);

	formatMenu->addSeparator();
	tb->addSeparator();


	//Align Left
	const QIcon leftIcon = QIcon(rsrcPath + "/editor/textleft.png");
	actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
	actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
	actionAlignLeft->setCheckable(true);
	actionAlignLeft->setPriority(QAction::LowPriority);

	//Align Center
	const QIcon centerIcon = QIcon(rsrcPath + "/editor/textcenter.png");
	actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
	actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
	actionAlignCenter->setCheckable(true);
	actionAlignCenter->setPriority(QAction::LowPriority);

	//Align Right
	const QIcon rightIcon = QIcon(rsrcPath + "/editor/textright.png");
	actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
	actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
	actionAlignRight->setCheckable(true);
	actionAlignRight->setPriority(QAction::LowPriority);

	//Align Justify
	const QIcon fillIcon = QIcon(rsrcPath + "/editor/textjustify.png");
	actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);
	actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
	actionAlignJustify->setCheckable(true);
	actionAlignJustify->setPriority(QAction::LowPriority);

	//Creating a new QActionGroup
	QActionGroup* alignGroup = new QActionGroup(this);
	connect(alignGroup, &QActionGroup::triggered, this, &TextEdit::textAlign);

	//Mantain user local settings for alignment
	if (QApplication::isLeftToRight()) {
		alignGroup->addAction(actionAlignLeft);
		alignGroup->addAction(actionAlignCenter);
		alignGroup->addAction(actionAlignRight);
	}
	else {
		alignGroup->addAction(actionAlignRight);
		alignGroup->addAction(actionAlignCenter);
		alignGroup->addAction(actionAlignLeft);
	}
	alignGroup->addAction(actionAlignJustify);

	//Add all actions to toolbar
	tb->addActions(alignGroup->actions());
	formatMenu->addActions(alignGroup->actions());

	tb->addSeparator();

	//Lists
	QMenu* menuList = new QMenu("List menu");
	listActions[standard] = menuList->addAction(tr("Standard"), this, [this]() { listStyle(standard); });
	listActions[standard]->setCheckable(true);
	listActions[standard]->setChecked(true);

	listActions[disc] = menuList->addAction(QIcon(rsrcPath + "/editor/disc.png"), tr("Bullet List - Disc"), this, [this]() { listStyle(disc); });
	listActions[disc]->setCheckable(true);
	listActions[disc]->setChecked(false);

	listActions[circle] = menuList->addAction(QIcon(rsrcPath + "/editor/circle.png"), tr("Bullet List - Circle"), this, [this]() { listStyle(circle); });
	listActions[circle]->setCheckable(true);
	listActions[circle]->setChecked(false);

	listActions[square] = menuList->addAction(QIcon(rsrcPath + "/editor/square.png"), tr("Bullet List - Square"), this, [this]() { listStyle(square); });
	listActions[square]->setCheckable(true);
	listActions[square]->setChecked(false);

	listActions[decimal] = menuList->addAction(QIcon(rsrcPath + "/editor/decimal.png"), tr("Ordered List - Decimal"), this, [this]() { listStyle(decimal); });
	listActions[decimal]->setCheckable(true);
	listActions[decimal]->setChecked(false);

	listActions[alpha] = menuList->addAction(QIcon(rsrcPath + "/editor/alpha.png"), tr("Ordered List - Alpha"), this, [this]() { listStyle(alpha); });
	listActions[alpha]->setCheckable(true);
	listActions[alpha]->setChecked(false);

	listActions[alphaupper] = menuList->addAction(QIcon(rsrcPath + "/editor/alphaupper.png"), tr("Ordered List - Uppercase alpha"), this, [this]() { listStyle(alphaupper); });
	listActions[alphaupper]->setCheckable(true);
	listActions[alphaupper]->setChecked(false);

	listActions[roman] = menuList->addAction(QIcon(rsrcPath + "/editor/roman.png"), tr("Ordered List - Roman"), this, [this]() { listStyle(roman); });
	listActions[roman]->setCheckable(true);
	listActions[roman]->setChecked(false);

	listActions[romanupper] = menuList->addAction(QIcon(rsrcPath + "/editor/romanupper.png"), tr("Ordered List - Uppercase roman"), this, [this]() { listStyle(romanupper); });
	listActions[romanupper]->setCheckable(true);
	listActions[romanupper]->setChecked(false);


	//Setup ToolButton
	listButton = new QToolButton();
	listButton->setMenu(menuList);
	listButton->setToolTip(tr("List style"));
	listButton->setPopupMode(QToolButton::MenuButtonPopup);
	listButton->setDefaultAction(listActions[disc]);

	listButton->setCheckable(true);

	listButton->setIcon(QIcon(rsrcPath + "/editor/list.png"));
	tb->addWidget(listButton);

	tb->addSeparator();

	//LineHeight
	QMenu* menuLineHeight = new QMenu("Line height menu");

	//1
	actionLineHeight100 = new QAction(tr("1"), this);
	actionLineHeight100->setCheckable(true);


	//1.15
	actionLineHeight115 = new QAction(tr("1.15"), this);
	actionLineHeight115->setCheckable(true);

	//1.5
	actionLineHeight150 = new QAction(tr("1.5"), this);
	actionLineHeight150->setCheckable(true);

	//2
	actionLineHeight200 = new QAction(tr("2"), this);
	actionLineHeight200->setCheckable(true);

	//Creating a new QActionGroup
	QActionGroup* lineHeightGroup = new QActionGroup(this);
	connect(lineHeightGroup, &QActionGroup::triggered, this, &TextEdit::setLineHeight);

	lineHeightGroup->addAction(actionLineHeight100);
	lineHeightGroup->addAction(actionLineHeight115);
	lineHeightGroup->addAction(actionLineHeight150);
	lineHeightGroup->addAction(actionLineHeight200);

	menuLineHeight->addActions(lineHeightGroup->actions());

	//Setup ToolButton
	lineHeightButton = new QToolButton();
	lineHeightButton->setMenu(menuLineHeight);
	lineHeightButton->setPopupMode(QToolButton::InstantPopup);
	lineHeightButton->setToolTip(tr("Line height"));

	lineHeightButton->setIcon(QIcon(rsrcPath + "/editor/lineheight.png"));

	tb->addWidget(lineHeightButton);

	formatMenu->addSeparator();

	/********** ACCOUNT MENU **********/
	addToolBarBreak(Qt::TopToolBarArea);

	tb = addToolBar(tr("&Account"));
	menu = menuBar()->addMenu(tr("&Account"));

	//Highlight user text
	const QIcon HighlightUsersIcon(rsrcPath + "/editor/highlightusers.png");
	actionHighlightUsers = menu->addAction(HighlightUsersIcon, tr("&Highlight users text"), this, &TextEdit::highlightUsersText);
	tb->addAction(actionHighlightUsers);
	actionHighlightUsers->setCheckable(true);

	//Edit profile
	const QIcon userIcon(rsrcPath + "/editor/user.png");
	a = menu->addAction(userIcon, tr("&Edit profile"), this, &TextEdit::openEditProfile);
	tb->addAction(a);

	//Close document
	tb->addAction(closeDocoumentAction);

	//Online users toolbar
	onlineUsersToolbar = new QToolBar(tr("&Online users"));
	addToolBar(Qt::RightToolBarArea, onlineUsersToolbar);

	/********** FONT AND SIZE **********/

	//Font and Size
	tb = addToolBar(tr("Font and Size"));
	tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBar(tb);

	//Combobox setup
	comboFont = new QFontComboBox(tb);
	tb->addWidget(comboFont);
	connect(comboFont, QOverload<const QString&>::of(&QComboBox::activated), this, &TextEdit::textFamily);

	//Size combobox
	comboSize = new QComboBox(tb);
	comboSize->setObjectName("comboSize");
	tb->addWidget(comboSize);
	comboSize->setEditable(true);

	//Adding standard sizes to combobox
	const QList<int> standardSizes = QFontDatabase::standardSizes();
	foreach(int size, standardSizes)
		comboSize->addItem(QString::number(size));

	//Update current index according to format
	comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

	connect(comboSize, QOverload<const QString&>::of(&QComboBox::activated), this, &TextEdit::textSize);


	const QIcon incrementSizeIcon = QIcon(rsrcPath + "/editor/incrementsize.png");
	a = new QAction(incrementSizeIcon, tr("Increment text size"), this);
	connect(a, &QAction::triggered, this, &TextEdit::incrementSize);

	tb->addAction(a);


	const QIcon decrementSizeIcon = QIcon(rsrcPath + "/editor/decrementsize.png");
	a = new QAction(decrementSizeIcon, tr("Decrement text size"), this);
	connect(a, &QAction::triggered, this, &TextEdit::decrementSize);

	tb->addAction(a);

	/********** FONT AND SIZE **********/
	menu = menuBar()->addMenu(tr("?"));
	menu->addAction(tr("Readme..."), this, &TextEdit::linkPressed);
	menu->addSeparator();
	menu->addAction(tr("About LiveText..."), _aboutwindow, &AboutWindow::exec);
}



/**************************** ONLINE USERS ****************************/
/*
*	Setup Online users toolbar
*	Setup current user
*	Add presence to editor
*	Remove presence from editor
*/

void TextEdit::setupOnlineUsersActions()
{
	QMap<qint32, Presence*>::iterator it;

	//Clear online users toolbar
	onlineUsersToolbar->clear();

	//Generate user toolbar for every user logged
	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		Presence* p = it.value();

		QAction* onlineAction = new QAction(QIcon(p->profilePicture()), p->name().toStdString().c_str(), this);
		connect(onlineAction, &QAction::triggered, this, &TextEdit::handleMultipleSelections);

		onlineAction->setCheckable(true);
		onlineUsersToolbar->addAction(onlineAction);

		p->setAction(onlineAction);
	}
}

//Slot to add a Presence in the editor
void TextEdit::newPresence(qint32 userId, QString username, QImage image)
{
	//Init a random value for new user color
	int randomNumber = 7 + (userId) % 11;

	//Pick a color
	QColor color = (Qt::GlobalColor) (randomNumber);

	//Getting user image
	QPixmap userPic;
	userPic.convertFromImage(image);

	//Remove presence if already present
	if (onlineUsers.contains(userId)) {
		removePresence(userId);
	}

	//Insert presence in editor
	onlineUsers.insert(userId, new Presence(username, color, userPic, _textEdit));

	//Redraw of the onlineUsers toolbar
	setupOnlineUsersActions();

	//Reset cursor postion to send to the new user current cursor position
	_currentCursorPosition = -1;

	//Recompute text highlighting
	updateUsersSelections();
}

//Remove presence in the document
void TextEdit::removePresence(qint32 userId)
{
	Presence* p = onlineUsers.find(userId).value();

	//Hide user cursor
	p->label()->clear();

	//Remove frome editor
	onlineUsers.remove(userId);

	//Redraw of the onlineUsers toolbar
	setupOnlineUsersActions();

	//Clean pointers
	delete p;
	p = nullptr;
}

/**************************** EDITOR UI/UX ****************************/
/*
*	Editor resize
*	Close document msgbox
*	Close editor
*	Status bar messages
*	Error messages
*	Start timer
*	Setting document filename, URI
*	File print, Print Preview
*	Save as PDF
*	Share document URI
*/

void TextEdit::resizeEditor(const QSizeF& newSize)
{
	int height = QApplication::desktop()->availableGeometry(this).height();
	if (newSize.height() > height) {
		_textEdit->setFixedHeight(newSize.height());
	}
	else {
		_textEdit->setFixedHeight(height);
	}
}

void TextEdit::askBeforeCloseDocument()
{
	QMessageBox::StandardButton reply = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("Do you want to close this document?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		_cursorTimer.stop();
		emit closeDocument(_user.getUserId());
	}
}

void TextEdit::showStatusBarMessage(QString text)
{
	//Show status bar
	statusBar()->show();

	//Shows message for 5"
	statusBar()->showMessage(text, STATUSBAR_MSG_DURATION);

	//Hides status bar after 5"
	QTimer::singleShot(STATUSBAR_MSG_DURATION, [this] {statusBar()->hide(); });
}

void TextEdit::closeDocumentError(QString error)
{
	showStatusBarMessage(error);
	//Restart timer to send
	_cursorTimer.start();
}


void TextEdit::closeEditor()
{
	const QSignalBlocker blocker(_textEdit->document());

	//Clear all users
	onlineUsers.clear();

	//Clear document (editor one, which allow user to write in)
	_textEdit->document()->clear();

	//Kill timer
	_cursorTimer.stop();

	if (_shareUri->isVisible()) {
		_shareUri->close();
	}

	//Close window
	this->close();
}


void TextEdit::criticalError(QString error)
{
	QMessageBox::StandardButton(QMessageBox::critical(this, QCoreApplication::applicationName(), error, QMessageBox::Ok));
}


void TextEdit::resetCursorPosition()
{
	//Set new cursor
	_textEdit->setTextCursor(QTextCursor(_textEdit->document()));
	area->verticalScrollBar()->setValue(area->verticalScrollBar()->minimum());
}

//	Handle timer event to send cursor position to the server
void TextEdit::startCursorTimer()
{
	_cursorTimer.callOnTimeout<TextEdit*>(this, &TextEdit::cursorTimerEvent);
	_cursorTimer.start(CURSOR_SEND_INTERVAL);
}


void TextEdit::cursorTimerEvent()
{
	//Sends cursor position only if it's different from previous sent position
	if (_textEdit->textCursor().position() != _currentCursorPosition) {
		_currentCursorPosition = _textEdit->textCursor().position();
		emit newCursorPosition(_user.getUserId(), _textEdit->textCursor().position());
	}
}


void TextEdit::setCurrentFileName(QString fileName, QString uri)
{
	this->fileName = fileName;
	this->URI = uri;

	_shareUri = new ShareUriWindow(URI);

	//Sulla finestra appare nomeFile - nomeApplicazione
	setWindowTitle(tr("%1 - %2").arg(fileName, QCoreApplication::applicationName()));
}


void TextEdit::filePrint()
{
#if QT_CONFIG(printdialog)
	//Create printer object
	QPrinter printer(QPrinter::HighResolution);

	//Open print dialog
	QPrintDialog* dlg = new QPrintDialog(&printer, this);

	//Enable option to print only selection
	if (_textEdit->textCursor().hasSelection())
		dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);

	//Open print document window
	dlg->setWindowTitle(tr("Print Document"));
	if (dlg->exec() == QDialog::Accepted)
		//If answer is yes, start printing
		_textEdit->print(&printer);
	delete dlg;
#endif
}

//Print preview
void TextEdit::filePrintPreview()
{
#if QT_CONFIG(printpreviewdialog)
	//Create printer
	QPrinter printer(QPrinter::HighResolution);

	//Print preview window
	QPrintPreviewDialog preview(&printer, this);

	//Connect to print function
	connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TextEdit::printPreview);

	preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter* printer)
{
#ifdef QT_NO_PRINTER
	Q_UNUSED(printer);
#else
	//Print from print preview
	_textEdit->print(printer);
#endif
}

//Save as PDF File
void TextEdit::filePrintPdf()
{
#ifndef QT_NO_PRINTER
	//Open Save As... window for saving PDF
	QFileDialog fileDialog(this, tr("Export PDF"));
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);

	//Show only PDF files
	fileDialog.setMimeTypeFilters(QStringList("application/pdf"));

	//Append pdf suffix to file
	fileDialog.setDefaultSuffix("pdf");

	//If answer is not Accept it closes
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString fileName = fileDialog.selectedFiles().first();

	//Open print window but with PDF settings
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);

	//Print document
	_textEdit->document()->print(&printer);

	//Show in status bar outcome of operation ("Exported path/to/file.pdf")
	showStatusBarMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(fileName)));
#endif
}

void TextEdit::fileShare()
{
	//Show created window
	if (_shareUri->exec() == QDialog::Accepted)
		showStatusBarMessage(tr("URI copied into clipboards"));		//Show message to clipboard
}

/**************************** LISTS ****************************/
/*
*	Handle button behavior, when a type of list is checked it's unchecks other styles
*	Create a new list
*	Add a block to list
*	Remove a block from a list
*/

void TextEdit::listStyle(int styleIndex)
{
	const QSignalBlocker blocker(_textEdit->document());

	//New list format
	QTextListFormat listFmt;

	//Getting document cursor
	QTextCursor cursor = _textEdit->textCursor();


	//Create standard style (undefined)
	QTextListFormat::Style style = QTextListFormat::ListStyleUndefined;

	//Check/Uncheck correct list menu entry and save style
	if (listActions[styleIndex]->isChecked()) {
		toggleCheckList(styleIndex);
		style = listStyles[styleIndex];
	}
	else {
		toggleCheckList(standard);
	}

	//Set new list style
	listFmt.setStyle(style);

	//Tells to server new list format
	emit toggleList(cursor.selectionStart(), cursor.selectionEnd(), listFmt);

}

void TextEdit::createList(int position, QTextListFormat fmt)
{
	const QSignalBlocker blocker(_textEdit->document());

	//Moving to desired position to create the list
	_extraCursor->setPosition(position);

	//If block is in a list, remove from that list
	if (_extraCursor->currentList())
		removeBlockFromList(position);

	//Setting list indentation to 1 step
	fmt.setIndent(1);

	//Creating list with given format
	_extraCursor->createList(fmt);

	updateEditorSelectedActions();
}

void TextEdit::removeBlockFromList(int blockPosition)
{
	const QSignalBlocker blocker(_textEdit->document());

	//Moving to target block
	_extraCursor->setPosition(blockPosition);

	//Getting current list
	QTextList* currentList = _extraCursor->currentList();

	//Getting current block format
	QTextBlockFormat blkFormat = _extraCursor->blockFormat();

	if (currentList) {
		//Getting current block
		QTextBlock blk = _extraCursor->block();

		//Remove target bock from list
		currentList->remove(blk);
	}

	//Makes the index of the blockFormat object -1 --> Reset block format to default
	blkFormat.setObjectIndex(-1);
	blkFormat.setIndent(0);

	//Apply new format (is applied anyway)
	_extraCursor->setBlockFormat(blkFormat);

	//Reload updated block format to send it to the server
	//blkFormat = _extraCursor->blockFormat();
	//Sends new block format to server
	//emit blockFormatChanged(blockPosition, blockPosition, blkFormat);

	//GUI update
	updateEditorSelectedActions();
}

void TextEdit::addBlockToList(int blockPosition, int listPosition)
{
	const QSignalBlocker blocker(_textEdit->document());

	//Getting block at blockPosition
	_extraCursor->setPosition(blockPosition);
	QTextBlock blk = _extraCursor->block();

	//If block is in a list, remove from that list
	if (_extraCursor->currentList())
		removeBlockFromList(blockPosition);

	//Getting list at listPosition
	_extraCursor->setPosition(listPosition);
	QTextList* currentList = _extraCursor->currentList();


	//Add block to list
	currentList->add(blk);

	//GUI update
	updateEditorSelectedActions();
}


/**************************** BLOCK FORMAT ****************************/
/*
*	Apply remote block format
*/

void TextEdit::applyBlockFormat(int position, QTextBlockFormat fmt)
{
	const QSignalBlocker blocker(_textEdit->document());

	_extraCursor->setPosition(position);

	//Sets alignment and indent in a new format (due to compatibility problems)
	QTextBlockFormat format;
	format.setAlignment(fmt.alignment());

	//Set format lineheight, if not present sets default one
	format.setIndent(fmt.indent());
	if (fmt.lineHeight() == 0)
		format.setLineHeight(100, QTextBlockFormat::ProportionalHeight);
	else
		format.setLineHeight(fmt.lineHeight(), QTextBlockFormat::ProportionalHeight);

	//Sets block format in current block
	_extraCursor->mergeBlockFormat(format);

	//Call alignment changed for update GUI
	alignmentChanged(fmt.alignment());

	//GUI update
	updateEditorSelectedActions();
}


/**************************** CHANGE TEXT FORMAT ****************************/
/*
*	Bold/Underline/Italic
*	Change text family
*	Change text size
*	Change text color
*	Change text alignment
*	Merge format on selection (if present)
*	Apply remote char format
*	Line height
*/

void TextEdit::textBold()
{
	const QSignalBlocker blocker(_textEdit->document());

	QTextCharFormat fmt;

	//If the Bold button is checked it sets correct text weight
	fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);

	//Apply format
	mergeFormatOnSelection(fmt);
}

void TextEdit::textUnderline()
{
	const QSignalBlocker blocker(_textEdit->document());

	//Set Underline according to button
	QTextCharFormat fmt;
	fmt.setFontUnderline(actionTextUnderline->isChecked());

	//Apply format
	mergeFormatOnSelection(fmt);
}

void TextEdit::textItalic()
{
	const QSignalBlocker blocker(_textEdit->document());

	//Set Italic according to button
	QTextCharFormat fmt;
	fmt.setFontItalic(actionTextItalic->isChecked());

	//Apply format
	mergeFormatOnSelection(fmt);
}

void TextEdit::textStrikeout()
{
	const QSignalBlocker blocker(_textEdit->document());

	//Set Strikethrough according to button
	QTextCharFormat fmt;
	fmt.setFontStrikeOut(actionTextStrikeout->isChecked());

	//Apply format
	mergeFormatOnSelection(fmt);
}


void TextEdit::textFamily(const QString& f)
{
	const QSignalBlocker blocker(_textEdit->document());

	//Set keyboard focus
	_textEdit->setFocus();

	//Set font family
	QTextCharFormat fmt;
	fmt.setFontFamily(f);

	//Apply format
	mergeFormatOnSelection(fmt);
}


void TextEdit::textSize(const QString& p)
{
	const QSignalBlocker blocker(_textEdit->document());

	//Set keyboard focus
	_textEdit->setFocus();

	//Casting point size to float
	qreal pointSize = p.toFloat();
	if (pointSize > 0) {
		//Set font size
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);

		//Apply format
		mergeFormatOnSelection(fmt);
	}
}

void TextEdit::incrementSize()
{
	//Get current combobox index
	int index = comboSize->currentIndex();
	if (index < comboSize->count() - 1) {
		//Increment it (if less the max size)
		comboSize->setCurrentIndex(++index);

		//Adapt text size according
		textSize(comboSize->currentText());
	}
}

void TextEdit::decrementSize()
{
	int index = comboSize->currentIndex();
	if (index > 0) {
		//Increment it (if greater then first index)
		comboSize->setCurrentIndex(--index);

		//Adapt text size according
		textSize(comboSize->currentText());
	}
}

void TextEdit::textColor()
{
	const QSignalBlocker blocker(_textEdit->document());

	//Open dialog window for colors
	QColor col = QColorDialog::getColor(_textEdit->textColor(), this);

	//Checks if color is valid
	if (!col.isValid())
		return;

	//Change format
	QTextCharFormat fmt;
	fmt.setForeground(col);

	//Apply format
	mergeFormatOnSelection(fmt);

	//Update GUI
	colorChanged(col);
}


void TextEdit::textAlign(QAction* a)
{
	const QSignalBlocker blocker(_textEdit->document());

	//Sets alignment according to what button is pressed
	if (a == actionAlignLeft)
		_textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	else if (a == actionAlignCenter)
		_textEdit->setAlignment(Qt::AlignHCenter);
	else if (a == actionAlignRight)
		_textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	else if (a == actionAlignJustify)
		_textEdit->setAlignment(Qt::AlignJustify);

	QTextCursor cursor = _textEdit->textCursor();

	//Reset previous cursor position so it is sent as soon as possible, to avoid issues
	//where the cursor is not properly positioned in the text after changing the alignment
	_currentCursorPosition = -1;

	//Sends new block alignment to server
	emit blockFormatChanged(cursor.selectionStart(), cursor.selectionEnd(),
		cursor.blockFormat().alignment());
}


//Merge new format to current char format also for a selection (if present) 
void TextEdit::mergeFormatOnSelection(const QTextCharFormat& format)
{
	//Getting document cursor
	QTextCursor cursor = _textEdit->textCursor();

	//Apply format to the document, if the cursor (_textEdit->textCursor()) has a selection, apply format to the selection
	_textEdit->mergeCurrentCharFormat(format);

	//Sends new char format to server (in case of selection it sends the updated format of every character because they can be different
	for (int i = cursor.selectionStart(); i < cursor.selectionEnd(); ++i) {
		_extraCursor->setPosition(i + 1);
		emit symbolFormatChanged(i, _extraCursor->charFormat());
	}
}


void TextEdit::applyCharFormat(int position, QTextCharFormat fmt)
{
	const QSignalBlocker blocker(_textEdit->document());

	_extraCursor->setPosition(position);
	_extraCursor->setPosition(position + 1, QTextCursor::KeepAnchor);

	//Apply char format to text
	_extraCursor->setCharFormat(fmt);

	//Update GUI buttons according to new format
	currentCharFormatChanged(_textEdit->textCursor().charFormat());
}


void TextEdit::setLineHeight(QAction* a)
{
	const QSignalBlocker blocker(_textEdit->document());

	QTextBlockFormat fmt;

	//Sets alignment according to what button is pressed
	if (a == actionLineHeight100)
		fmt.setLineHeight(100, QTextBlockFormat::ProportionalHeight);
	else if (a == actionLineHeight115)
		fmt.setLineHeight(115, QTextBlockFormat::ProportionalHeight);
	else if (a == actionLineHeight150)
		fmt.setLineHeight(150, QTextBlockFormat::ProportionalHeight);
	else if (a == actionLineHeight200)
		fmt.setLineHeight(200, QTextBlockFormat::ProportionalHeight);;

	//Sets block format
	QTextCursor cursor = _textEdit->textCursor();
	cursor.mergeBlockFormat(fmt);
	fmt = cursor.blockFormat();

	//Sends new block lineHeight to server
	emit blockFormatChanged(cursor.selectionStart(), cursor.selectionEnd(),
		fmt.lineHeight(), fmt.lineHeightType());
}

/**************************** GUI UPDATE ****************************/
/*
*	If there's a change in current format it updates buttons/comboboxes according to new format
*/

void TextEdit::cursorPositionChanged()
{

	//Update scrollbar position according to cursor position wiith offsets
	int cursorPosition = _textEdit->cursorRect().y();
	int areaBottom = area->contentsRect().bottom();
	int scrollValue = area->verticalScrollBar()->value();
	int scrollOffset = areaBottom / 6;

	if (_textEdit->textCursor().position() == 0)
		area->verticalScrollBar()->setValue(area->verticalScrollBar()->minimum());
	else if (cursorPosition >= areaBottom + scrollValue - 40)
		area->verticalScrollBar()->setValue(scrollValue + scrollOffset);
	else if (cursorPosition && cursorPosition <= scrollValue + 25)
		area->verticalScrollBar()->setValue(scrollValue - scrollOffset);

	updateEditorSelectedActions();
}


void TextEdit::updateEditorSelectedActions()
{
	QTextCursor cursor = _textEdit->textCursor();

	//If there's a selection and the selection is from right to left
	//It take the format of the first character of the selection
	if (cursor.hasSelection() && (cursor.selectionStart() == cursor.position()))
		cursor.setPosition(cursor.position() + 1); //Format is taken from the char left to the cursor

	//Users cursors
	redrawAllCursors();

	//Block format
	QTextBlockFormat blockFmt = cursor.blockFormat();

	//Alignment
	alignmentChanged(blockFmt.alignment());

	//Line height
	lineHeightChanged(blockFmt.lineHeight());

	//Text format
	currentCharFormatChanged(cursor.charFormat());

	//Lists
	QTextList* list = cursor.currentList();

	//Checks list format (if in list) and updates GUI according to format
	if (list) {
		for (int i = 0; i < 9; i++) {
			if (list->format().style() == listStyles[i]) {
				//Find list format to update GUI correctly
				toggleCheckList(i);
				break;
			}
		}
	}
	else {
		toggleCheckList(standard);
	}

}


//Checks only listType menu entry in list menu
void TextEdit::toggleCheckList(int listType)
{

	//All other entries are unchecked
	for (int i = 0; i < 9; ++i) {
		listActions[i]->setChecked(i == listType);
	}

	//Change behavior of main list button
	if (listType == standard)
		listButton->setDefaultAction(listActions[disc]);
	else
		listButton->setDefaultAction(listActions[listType]);

	//Change icon of list button according to format
	listButton->setChecked(listType != standard);
	listButton->setIcon(QIcon(rsrcPath + listIconPath[listType]));
}


void TextEdit::currentCharFormatChanged(const QTextCharFormat& format)
{
	//Apply changes to buttons/comboboxes
	fontChanged(format.font());
	colorChanged(format.foreground().color());
}

void TextEdit::fontChanged(const QFont& f)
{
	//Adapt size and font combobox according to current format
	comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));

	//Check/Uncheck Bold/Italic/Underline buttons
	actionTextBold->setChecked(f.bold());
	actionTextItalic->setChecked(f.italic());
	actionTextUnderline->setChecked(f.underline());
	actionTextStrikeout->setChecked(f.strikeOut());
}

void TextEdit::colorChanged(const QColor& c)
{
	//Change color of textColor button according to text
	QPixmap pix(rsrcPath + "/editor/textcolor.png");
	QBitmap mask = pix.createMaskFromColor(Qt::transparent, Qt::MaskInColor);
	pix.fill(c);
	pix.setMask(mask);
	actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(Qt::Alignment a)
{
	//Checks alignment button according to Alignment a
	if (a & Qt::AlignLeft)
		actionAlignLeft->setChecked(true);
	else if (a & Qt::AlignHCenter)
		actionAlignCenter->setChecked(true);
	else if (a & Qt::AlignRight)
		actionAlignRight->setChecked(true);
	else if (a & Qt::AlignJustify)
		actionAlignJustify->setChecked(true);
}

void TextEdit::lineHeightChanged(qreal height)
{
	//Checks line height button according to Alignment a
	if (height == 115)
		actionLineHeight115->setChecked(true);
	else if (height == 150)
		actionLineHeight150->setChecked(true);
	else if (height == 200)
		actionLineHeight200->setChecked(true);
	else //Default line height
		actionLineHeight100->setChecked(true);

}

/**************************** PASTE EVENT ****************************/
/*
*	When clipboard content changes, it prevents pasting images
*/

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD

	//Se ho del testo negli appunti allora si sblocca il pulsante incolla
	if (const QMimeData* md = QApplication::clipboard()->mimeData())
		actionPaste->setEnabled(!md->hasImage());

#endif
}


//Disable drag&drop images
bool QTextEditWrapper::canInsertFromMimeData(const QMimeData* source) const {
	return !(source->hasImage() || source->hasUrls());
}


//Prevent pasting images overload of QTextEdit function
void QTextEditWrapper::insertFromMimeData(const QMimeData* source) {
	if (!source->hasImage())
		QTextEdit::insertFromMimeData(source);
}


/**************************** CONTENTS CHANGE ****************************/
/*
*	This function handles the character insertion/deletion of the document.
*	It's also intercepts all list/block modifications, copy/paste actions, and sets the correct formats.
*	Handle new char from server and delete char from server
*/

void TextEdit::contentsChange(int position, int charsRemoved, int charsAdded)
{
	/*************** CHARACTER DELETION ***************/

	/*if (charsRemoved == 1)
	{
		emit charDeleted(position);
	}
	else // if (charsRemoved > 1)
	{
		for (int i = 0; i < charsRemoved; i += BULK_EDIT_SIZE)
		{
			emit charGroupDeleted(position, std::min<int>(BULK_EDIT_SIZE, charsRemoved - i));
		}
	}*/

	/*************** CHARACTER INSERTION ***************/

	/*if (charsAdded == 1)
	{
		//Cursor position is set to i + 1 to get correct character format
		_extraCursor->setPosition(position + 1);

		//Getting QTextCharFormat from cursor
		QTextCharFormat fmt = _extraCursor->charFormat();

		//Reset position to i to get blockformat
		_extraCursor->setPosition(position);

		//Getting QTextBlockFormat from cursor
		QTextBlockFormat blockFmt = _extraCursor->blockFormat();

		//Getting inserted character
		QChar ch = _textEdit->document()->characterAt(position);

		emit charInserted(ch, fmt, position, false);

		//If it is a paragraph separator ('8233')
		if (ch == QChar::ParagraphSeparator)
		{
			//Update block format to server
			emit blockFormatChanged(position, position, blockFmt);
		}
	}
	else // if (charsAdded > 1)
	{
		QList<QChar> bulkChars;
		QList<QTextCharFormat> bulkFormats;

		for (int i = position, pos = position; i < position + charsAdded; ++i)
		{
			//Cursor position is set to i + 1 to get correct character format
			_extraCursor->setPosition(i + 1);

			//Getting QTextCharFormat from cursor
			QTextCharFormat fmt = _extraCursor->charFormat();

			//Reset position to i to get blockformat
			_extraCursor->setPosition(i);

			//Getting inserted character
			QChar ch = _textEdit->document()->characterAt(i);

			//Skip null characters '\0'
			if (ch != QChar::Null)
			{
				bulkChars.append(ch);
				bulkFormats.append(fmt);

				// Send a group of characters either if the end of the block or the buffer size value is reached
				if (ch == QChar::ParagraphSeparator || bulkChars.length() == BULK_EDIT_SIZE || i == position + charsAdded - 1)
				{
					//Getting QTextBlockFormat from cursor
					QTextBlockFormat blockFmt = _extraCursor->blockFormat();
					blockFmt.lineHeight();

					//Boolean flag to check if the bulk contains the last '8233' of the document (eof)
					bool isLast = (ch == QChar::ParagraphSeparator) && (i == _textEdit->document()->characterCount() - 1);
					emit charGroupInserted(bulkChars, bulkFormats, pos, isLast, blockFmt);

					pos += bulkChars.length();
					bulkChars.clear();			 // Clear buffers before the next iteration (block to be inserted)
					bulkFormats.clear();
				}

				//If it is a paragraph separator ('8233')
				if (ch == QChar::ParagraphSeparator)
				{
					//Getting current block and list (if present)
					QTextBlock currentBlock = _extraCursor->block();
					QTextList* textList = _extraCursor->currentList();

					if (textList)
					{
						//Getting first block of the list
						QTextBlock firstListBlock = textList->item(0);

						//If the current block is the beginning of a new list emit the signal to create a new one
						if (currentBlock == firstListBlock)
						{
							// It's not actually a new list in this case, but the re-insertion of a block
							if (textList->count() > 1 && i == position + charsAdded - 1)
							{
								QTextBlock otherListBlock = textList->item(1);
								emit assignBlockToList(currentBlock.position(), otherListBlock.position());
							}
							else
								emit createNewList(currentBlock.position(), textList->format());
						}

						//Else assign current block to his proper list
						else
							emit assignBlockToList(currentBlock.position(), firstListBlock.position());
					}
					else
						emit setBlockNoList(currentBlock.position());
				}
			}
		}

	}*/

	for (int i = 0; i < charsRemoved; ++i) {
		emit charDeleted(position);
	}

	QTextBlockFormat blockFmt;


	for (int i = position; i < position + charsAdded; ++i) {
		//Cursor position is set to i + 1 to get correct character format
		_extraCursor->setPosition(i + 1);

		//Getting QTextCharFormat from cursor
		QTextCharFormat fmt = _extraCursor->charFormat();

		//Reset position to i to get blockformat
		_extraCursor->setPosition(i);

		//Getting QTextBlockFormat from cursor
		blockFmt = _extraCursor->blockFormat();

		//Getting inserted character
		QChar ch = _textEdit->document()->characterAt(i);

		//Emit signal to DocumentEditor to insert a character if is not a null character '\0'
		if (ch != QChar::Null) {
			//Boolean flag to check id it's last '8233' of the document (eof)
			bool isLast = (ch == QChar::ParagraphSeparator) && (i == _textEdit->document()->characterCount() - 1);
			emit charInserted(ch, fmt, i, isLast);
		}

		//If it is a paragraph separator ('8233')
		if (ch == QChar::ParagraphSeparator) {

			//Update block format to server
			emit blockFormatChanged(i, i, blockFmt);


			//Getting current block and list (if present)
			QTextBlock currentBlock = _extraCursor->block();
			QTextList* textList = _extraCursor->currentList();

			if (textList)
			{
				//Getting first block of the list
				QTextBlock firstListBlock = textList->item(0);

				//If the current block is the beginning of a new list emit the signal to create a new one
				if (currentBlock == firstListBlock)
				{
					// It's not actually a new list in this case, but the re-insertion of a block
					if (textList->count() > 1 && i == position + charsAdded - 1)
					{
						QTextBlock otherListBlock = textList->item(1);
						emit assignBlockToList(currentBlock.position(), otherListBlock.position());
					}
					else
						emit createNewList(currentBlock.position(), textList->format());
				}

				//Else assign current block to his proper list
				else
					emit assignBlockToList(currentBlock.position(), firstListBlock.position());
			}
			else
				emit setBlockNoList(currentBlock.position());
		}
	}

	//If in my insertion there's not a 8233 (\n), i apply the correct block format
	if (charsAdded > 1) {
		emit blockFormatChanged(position + charsAdded - 1, position + charsAdded - 1, blockFmt);
	}


	//Update GUI after some insertion/deletion
	cursorPositionChanged();

	//User text higlighting
	updateUsersSelections();
}


void TextEdit::newChar(QChar ch, QTextCharFormat format, int position)
{
	const QSignalBlocker blocker(_textEdit->document());

	_extraCursor->setPosition(position);

	//Set new char format
	_extraCursor->setCharFormat(format);

	//Insert character at position
	_extraCursor->insertText(ch);

	//GUI update
	updateEditorSelectedActions();

	//User text higlighting
	updateUsersSelections();
}

void TextEdit::removeChar(int position)
{
	const QSignalBlocker blocker(_textEdit->document());

	_extraCursor->setPosition(position);

	//Delete character
	_extraCursor->deleteChar();

	//GUI update
	updateEditorSelectedActions();

	//User text higlighting
	updateUsersSelections();
}

/**************************** EXTRA CURSORS ****************************/
/*
*	Functions to handle extra cursors position updates.
*	Recomputes all positions based on document scroll positions.
*	Handle all user's cursor movements
*/

void TextEdit::userCursorPositionChanged(qint32 position, qint32 user)
{
	//Finds the Presence
	Presence* p = onlineUsers.find(user).value();

	if (position < _textEdit->document()->characterCount()) {
		//Change user's cursor position
		p->cursor()->setPosition(position);
	}
	else {
		p->cursor()->setPosition(_textEdit->document()->characterCount() - 1);
	}

	drawGraphicCursor(p);
}

//Redraw all cursor in case of window update (scroll, resize...)
void TextEdit::redrawAllCursors() {

	QMap<qint32, Presence*>::iterator it;

	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		if (it.key() != _user.getUserId()) {

			Presence* p = it.value();

			drawGraphicCursor(p);
		}
	}
}

void TextEdit::drawGraphicCursor(Presence* p)
{
	//Getting Presence's cursor
	QTextCursor* cursor = p->cursor();
	//Getting Presence's QLabel (to draw the graphic cursor on the editor)
	QLabel* userCursorLabel = p->label();

	//Hide label to move it
	userCursorLabel->close();

	//Getting cursor rectangle (x,y) position of the window
	const QRect qRect = _textEdit->cursorRect(*cursor);

	//Draw the cursor (Pixmap)
	QPixmap pix(qRect.width() * 2.5, qRect.height());
	pix.fill(p->color());

	//Set the graphic cursor to the label
	userCursorLabel->setPixmap(pix);

	//Move label to the right coordinates
	userCursorLabel->move(qRect.left(), qRect.top());

	//Show the moved label
	userCursorLabel->show();
}

/**************************** HIGHLIGHT ONLINE USERS TEXT ****************************/
/*
*	Handle button behavior to check/uncheck all selections
*	Compute text highlighting selections
*	Handle resets of all text highlight
*/

void TextEdit::highlightUsersText()
{

	QMap<qint32, Presence*>::iterator it;

	//For every user it's check/uncheck his selection to be displayed
	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		Presence* p = it.value();

		p->actionHighlightText()->setChecked(actionHighlightUsers->isChecked());
	}

	updateUsersSelections();
}

void TextEdit::updateUsersSelections()
{
	if (areUserIconActive()) {
		_usersText.clear();

		//Generate extraSelections according to users text
		emit generateExtraSelection();

		//Shows user text highlight
		handleMultipleSelections();
	}
	else {
		//Clear all extra selections
		_textEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());

	}
}

//Sets and save all extra selections in the document
void TextEdit::setExtraSelections(qint32 userId, QPair<int, int> selection)
{

	if (onlineUsers.contains(userId)) {
		Presence* p = onlineUsers.find(userId).value();

		if (p->actionHighlightText()->isChecked()) {
			QTextCursor cursor(_textEdit->document());

			//Text selection
			cursor.setPosition(selection.first);
			cursor.setPosition(selection.second, QTextCursor::KeepAnchor);

			QTextEdit::ExtraSelection userText;

			//Sets format of extra selection
			QColor color = p->color();
			color.setAlpha(70);
			userText.format.setBackground(color);
			userText.cursor = cursor;

			//Add extra selection to Presence
			_usersText.append(userText);
		}
	}
}

//Sets extra selection in the editor based on what user is checked
void TextEdit::handleMultipleSelections()
{
	//Clear all extra selections
	_textEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());


	int actionsChecked = 0;
	foreach(Presence * p, onlineUsers.values()) {
		if (p->actionHighlightText()->isChecked())
			actionsChecked++;
	}

	//Check/Uncheck of actionHighlightUsers based on single highlight checked
	if (actionsChecked == 0) {
		actionHighlightUsers->setChecked(false);
	}

	if (actionsChecked == onlineUsers.size()) {
		actionHighlightUsers->setChecked(true);
	}

	//Sets the formatted selections in the editor
	_textEdit->setExtraSelections(_usersText);
}

bool TextEdit::areUserIconActive()
{
	foreach(Presence * p, onlineUsers.values()) {
		if (p->actionHighlightText()->isChecked())
			return true;
	}

	return false;
}




/**************************** ABOUT ****************************/
/*
*	Open about dialog
*	Open repository main page
*/


void TextEdit::linkPressed()
{
	QDesktopServices::openUrl(QUrl("https://github.com/paolo257428/livetext-pds/blob/master/README.md"));
}