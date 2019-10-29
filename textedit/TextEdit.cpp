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

TextEdit::TextEdit(User& user, QWidget* parent) : QMainWindow(parent), timerId(-1), _user(user)
{
	/**************************** GUI SETUP ****************************/

	setupMainWindow();
	setupFileActions();
	setupEditActions();
	setupTextActions();
	setupShareActions();
	setupUserActions();


	/**************************** CONNECTS ****************************/

	//GUI update in case of format change or cursor position changed
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::currentCharFormatChanged);
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::cursorPositionChanged);


	//Online users cursor redraw in case of window aspect, char format, cursor position changed
	connect(textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::redrawAllCursors);
	connect(textEdit->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::redrawAllCursors);

	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::redrawAllCursors);

	//Online users text highlight redraw in case of window aspect, char format, cursor position changed
	connect(textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::updateUsersSelections);

	//Mandatory to intercept character insertion, the document emit this signal every time a character inside document is added/removed
	connect(textEdit->document(), &QTextDocument::contentsChange, this, &TextEdit::contentsChange);

	//Adapt textEditor layout according to document
	connect(textEdit->document()->documentLayout(), &QAbstractTextDocumentLayout::documentSizeChanged, this, &TextEdit::resizeEditor);


	//If i can use clipboard setup connects from QTextEdit of cut/copy and clipboard management
#ifndef QT_NO_CLIPBOARD
	connect(textEdit, &QTextEdit::copyAvailable, actionCut, &QAction::setEnabled);
	connect(textEdit, &QTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);
	connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEdit::clipboardDataChanged);
#endif

	//Setup extra cursor
	_extraCursor = new QTextCursor(textEdit->document());

	/**************************** EDITOR INITIALIZATION ****************************/

	//Set default character and size
	QFont textFont("Helvetica");
	textFont.setStyleHint(QFont::SansSerif);
	textFont.setPointSize(12);
	textEdit->setFont(textFont);
	textEdit->document()->setDefaultFont(textFont);

	//Disable undo/redo
	textEdit->setUndoRedoEnabled(false);

	//Set document margin
	textEdit->document()->setDocumentMargin(64);

	//Initialize GUI, activating buttons according to current conditions
	fontChanged(textEdit->font());
	colorChanged(textEdit->textColor());
	alignmentChanged(textEdit->alignment());

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

	resize(availableGeometry.width() * 0.6, availableGeometry.height() * 2 / 3);
	move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);

	//Inizialize Qt text editor
	textEdit = new QTextEdit();
	textEdit->setMaximumWidth(width());
	textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//Creates 3D effect of document
	textEdit->setFrameStyle(QFrame::Plain);
	//textEdit->setFrameShadow(QFrame::Raised);
	textEdit->setLineWidth(1);

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
	bl->setContentsMargins(0, 20, 0, 20);
	bl->setAlignment(Qt::AlignHCenter); 	//Set widget alignment
	bl->addWidget(textEdit);	//Add text editor to widget

	//set widget layout
	cntr->setLayout(bl);

	//Set QMainWindow central widget
	setCentralWidget(area);

	//Hide status bar
	statusBar()->hide();

	//Makes keyboard focused on text editor
	textEdit->setFocus();
}



void TextEdit::setupFileActions()
{
	//New toolbar
	QToolBar* tb = addToolBar(tr("File Actions"));

	//New menu
	QMenu* menu = menuBar()->addMenu(tr("&File"));

	//If the print plugin is enabled
#ifndef QT_NO_PRINTER

	//Export document in PDF
	const QIcon exportPdfIcon = QIcon(rsrcPath + "/editor/exportpdf.png");
	QAction* a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
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

	menu->addSeparator();
#endif
}

void TextEdit::setupShareActions()
{
	QToolBar* tb = addToolBar(tr("Share"));
	QMenu* menu = menuBar()->addMenu(tr("&Share"));

	//Share URI, opens a box with URI pasted inside
	const QIcon shareIcon = QIcon(rsrcPath + "/editor/share.png");
	QAction* a = menu->addAction(shareIcon, tr("&Share URI"), this, &TextEdit::fileShare);
	tb->addAction(a);
}


void TextEdit::setupEditActions()
{
	QToolBar* tb = addToolBar(tr("Edit Actions"));
	QMenu* menu = menuBar()->addMenu(tr("&Edit"));


#ifndef QT_NO_CLIPBOARD
	//Cut
	const QIcon cutIcon = QIcon(rsrcPath + "/editor/editcut.png");
	actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &QTextEdit::cut);
	actionCut->setPriority(QAction::LowPriority);
	actionCut->setShortcut(QKeySequence::Cut);
	tb->addAction(actionCut);

	//Copy
	const QIcon copyIcon = QIcon(rsrcPath + "/editor/editcopy.png");
	actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &QTextEdit::copy);
	actionCopy->setPriority(QAction::LowPriority);
	actionCopy->setShortcut(QKeySequence::Copy);
	tb->addAction(actionCopy);

	//Paste
	const QIcon pasteIcon = QIcon(rsrcPath + "/editor/editpaste.png");
	actionPaste = menu->addAction(pasteIcon, tr("&Paste"), textEdit, &QTextEdit::paste);
	actionPaste->setPriority(QAction::LowPriority);
	actionPaste->setShortcut(QKeySequence::Paste);
	tb->addAction(actionPaste);

	//Checks if there's some items in the clipboards
	clipboardDataChanged();
#endif
}

void TextEdit::setupTextActions()
{
	QToolBar* tb = addToolBar(tr("Format Actions"));
	QMenu* menu = menuBar()->addMenu(tr("F&ormat"));

	//Bold
	const QIcon boldIcon = QIcon(rsrcPath + "/editor/textbold.png");
	actionTextBold = menu->addAction(boldIcon, tr("&Bold"), this, &TextEdit::textBold);
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
	actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);
	actionTextItalic->setPriority(QAction::LowPriority);
	actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
	actionTextItalic->setCheckable(true);

	QFont italic;
	italic.setItalic(true);
	actionTextItalic->setFont(italic);

	tb->addAction(actionTextItalic);


	//Underline
	const QIcon underlineIcon = QIcon(rsrcPath + "/editor/textunder.png");
	actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);
	actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
	actionTextUnderline->setPriority(QAction::LowPriority);
	actionTextUnderline->setCheckable(true);

	QFont underline;
	underline.setUnderline(true);
	actionTextUnderline->setFont(underline);

	tb->addAction(actionTextUnderline);

	//Strikethrough
	const QIcon strikeIcon = QIcon(rsrcPath + "/editor/textstrikethrough.png");
	actionTextStrikethrough = menu->addAction(strikeIcon, tr("&Strikethrough"), this, &TextEdit::textStrikethrough);
	actionTextStrikethrough->setPriority(QAction::LowPriority);
	actionTextStrikethrough->setCheckable(true);

	QFont strikethrough;
	strikethrough.setStrikeOut(true);
	actionTextStrikethrough->setFont(strikethrough);

	tb->addAction(actionTextStrikethrough);

	menu->addSeparator();
	tb->addSeparator();


	/***** ALIGNMENT *****/
	//Left
	const QIcon leftIcon = QIcon(rsrcPath + "/editor/textleft.png");
	actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
	actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
	actionAlignLeft->setCheckable(true);
	actionAlignLeft->setPriority(QAction::LowPriority);

	//Center
	const QIcon centerIcon = QIcon(rsrcPath + "/editor/textcenter.png");
	actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
	actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
	actionAlignCenter->setCheckable(true);
	actionAlignCenter->setPriority(QAction::LowPriority);

	//Right
	const QIcon rightIcon = QIcon(rsrcPath + "/editor/textright.png");
	actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
	actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
	actionAlignRight->setCheckable(true);
	actionAlignRight->setPriority(QAction::LowPriority);

	//Justify
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
	menu->addActions(alignGroup->actions());

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

	menu->addSeparator();
	tb->addSeparator();

	//Color
	QPixmap pix(rsrcPath + "/editor/textcolor.png");
	actionTextColor = menu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);
	tb->addAction(actionTextColor);

	menu->addSeparator();
	tb->addSeparator();

	//Highlight user text
	const QIcon HighlightUsersIcon(rsrcPath + "/editor/highlightusers.png");
	actionHighlightUsers = menu->addAction(HighlightUsersIcon, tr("&Highlight users text"), this, &TextEdit::highlightUsersText);
	tb->addAction(actionHighlightUsers);
	actionHighlightUsers->setCheckable(true);


	//Font and Size
	tb = addToolBar(tr("Font and Size"));
	tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBarBreak(Qt::TopToolBarArea);
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
	QAction* a = new QAction(incrementSizeIcon, tr("Increment text size"), this);
	connect(a, &QAction::triggered, this, &TextEdit::incrementSize);

	tb->addAction(a);


	const QIcon decrementSizeIcon = QIcon(rsrcPath + "/editor/decrementsize.png");
	a = new QAction(decrementSizeIcon, tr("Decrement text size"), this);
	connect(a, &QAction::triggered, this, &TextEdit::decrementSize);

	tb->addAction(a);
}

void TextEdit::setupUserActions()
{
	QToolBar* tb = addToolBar(tr("&Account"));
	QMenu* menu = menuBar()->addMenu(tr("&Account"));

	//Edit profile
	const QIcon userIcon(rsrcPath + "/editor/user.png");
	QAction* a = menu->addAction(userIcon, tr("&Edit profile"), this, &TextEdit::openEditProfile);
	tb->addAction(a);

	//Close document
	const QIcon closeDocumentIcon(rsrcPath + "/misc/logout.png");
	a = menu->addAction(closeDocumentIcon, tr("&Close Document"), this, &TextEdit::askBeforeCloseDocument);
	tb->addAction(a);

	//Online users toolbar
	onlineUsersToolbar = new QToolBar(tr("&Online users"));
	addToolBar(Qt::RightToolBarArea, onlineUsersToolbar);
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
	onlineUsers.insert(userId, new Presence(username, color, userPic, textEdit));

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
	if (newSize.height() > height * 0.78) {
		textEdit->setFixedHeight(newSize.height());
		area->verticalScrollBar()->setValue(area->verticalScrollBar()->maximum());
	}
	else {
		textEdit->setFixedHeight(height);
		area->verticalScrollBar()->setValue(area->verticalScrollBar()->minimum());
	}
}

void TextEdit::askBeforeCloseDocument()
{
	QMessageBox::StandardButton reply = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("Do you want to close this document?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		killTimer(timerId);
		emit closeDocument(_user.getUserId());
	}
}

void TextEdit::showStatusBarMessage(QString text)
{
	//Show status bar
	statusBar()->show();

	//Shows message for 5"
	statusBar()->showMessage(text, 5000);

	//Hides status bar after 5"
	QTimer::singleShot(5000, [this] {statusBar()->hide(); });
}

void TextEdit::closeDocumentError(QString error)
{
	showStatusBarMessage(error);
	startTimer(timerId);
}


void TextEdit::closeEditor()
{
	const QSignalBlocker blocker(textEdit->document());

	//Clear all users
	onlineUsers.clear();

	//Clear document (editor one, which allow user to write in)
	textEdit->document()->clear();

	//Kill timer
	if (timerId > 0)
		killTimer(timerId);

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
	textEdit->setTextCursor(QTextCursor(textEdit->document()));
}

//	Handle timer event to send cursor position to the server
void TextEdit::startCursorTimer()
{
	timerId = startTimer(CURSOR_SEND_INTERVAL);
	qDebug() << "Started timer with ID = " << timerId;
}

//TODO: Uncomment before release
void TextEdit::timerEvent(QTimerEvent* event)
{
	//Sends cursor position only if it's different from previous sent position
	//if (textEdit->textCursor().position() != _currentCursorPosition) {
	_currentCursorPosition = textEdit->textCursor().position();
	emit newCursorPosition(_user.getUserId(), textEdit->textCursor().position());
	//}
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
	if (textEdit->textCursor().hasSelection())
		dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);

	//Open print document window
	dlg->setWindowTitle(tr("Print Document"));
	if (dlg->exec() == QDialog::Accepted)
		//If answer is yes, start printing
		textEdit->print(&printer);
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
	textEdit->print(printer);
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
	textEdit->document()->print(&printer);

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
	const QSignalBlocker blocker(textEdit->document());

	//New list format
	QTextListFormat listFmt;

	//Getting document cursor
	QTextCursor cursor = textEdit->textCursor();


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
	const QSignalBlocker blocker(textEdit->document());

	//Moving to desired position to create the list
	_extraCursor->setPosition(position);

	//If block is in a list, remove from that list
	if (_extraCursor->currentList())
		removeBlockFromList(position);

	qDebug() << "Creating list at position " << position;

	//Setting list indentation to 1 step
	fmt.setIndent(1);

	//Creating list with given format
	_extraCursor->createList(fmt);

	cursorPositionChanged();
}

void TextEdit::removeBlockFromList(int blockPosition)
{
	const QSignalBlocker blocker(textEdit->document());

	//Moving to target block
	_extraCursor->setPosition(blockPosition);

	//Getting current list
	QTextList* currentList = _extraCursor->currentList();

	//Getting current block format
	QTextBlockFormat blkFormat = _extraCursor->blockFormat();

	if (currentList) {
		qDebug() << "Removing block at position " << blockPosition;

		//Getting current block
		QTextBlock blk = _extraCursor->block();

		//Remove target bock from list
		currentList->remove(blk);
	}

	//Makes the index of the blockFormat object -1 --> Reset block format to default
	blkFormat.setObjectIndex(-1);

	//Apply new format (is applied anyway)
	_extraCursor->setBlockFormat(blkFormat);

	//Reload updated block format to send it to the server
	blkFormat = _extraCursor->blockFormat();

	//Sends new block format to server
	emit blockFormatChanged(blockPosition, blockPosition, blkFormat);

	cursorPositionChanged();
}

void TextEdit::addBlockToList(int blockPosition, int listPosition)
{
	const QSignalBlocker blocker(textEdit->document());

	qDebug() << "Add block at position " << blockPosition << " to list at position " << listPosition;

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
	cursorPositionChanged();
}


/**************************** BLOCK FORMAT ****************************/
/*
*	Apply remote block format
*/

void TextEdit::applyBlockFormat(int position, QTextBlockFormat fmt)
{
	const QSignalBlocker blocker(textEdit->document());

	_extraCursor->setPosition(position);

	//Sets alignment and indent in a new format (due to compatibility problems)
	QTextBlockFormat format;
	format.setAlignment(fmt.alignment());

	//Set format lineheight, if not present sets default one
	format.setIndent(fmt.indent());
	if (fmt.lineHeight() == 0)
		format.setLineHeight(115, QTextBlockFormat::ProportionalHeight);
	else
		format.setLineHeight(fmt.lineHeight(), QTextBlockFormat::ProportionalHeight);

	//Sets block format in current block
	_extraCursor->mergeBlockFormat(format);

	//Call alignment changed for update GUI
	alignmentChanged(fmt.alignment());

	//GUI update
	cursorPositionChanged();
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
	const QSignalBlocker blocker(textEdit->document());

	QTextCharFormat fmt;

	//If the Bold button is checked it sets correct text weight
	fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);

	//Apply format
	mergeFormatOnSelection(fmt);
}

void TextEdit::textUnderline()
{
	const QSignalBlocker blocker(textEdit->document());

	//Set Underline according to button
	QTextCharFormat fmt;
	fmt.setFontUnderline(actionTextUnderline->isChecked());

	//Apply format
	mergeFormatOnSelection(fmt);
}

void TextEdit::textItalic()
{
	const QSignalBlocker blocker(textEdit->document());

	//Set Italic according to button
	QTextCharFormat fmt;
	fmt.setFontItalic(actionTextItalic->isChecked());

	//Apply format
	mergeFormatOnSelection(fmt);
}

void TextEdit::textStrikethrough()
{
	const QSignalBlocker blocker(textEdit->document());

	//Set Strikethrough according to button
	QTextCharFormat fmt;
	fmt.setFontStrikeOut(actionTextStrikethrough->isChecked());

	//Apply format
	mergeFormatOnSelection(fmt);
}


void TextEdit::textFamily(const QString& f)
{
	const QSignalBlocker blocker(textEdit->document());

	//Set font family
	QTextCharFormat fmt;
	fmt.setFontFamily(f);

	//Apply format
	mergeFormatOnSelection(fmt);
}


void TextEdit::textSize(const QString& p)
{
	const QSignalBlocker blocker(textEdit->document());

	//Set keyboard focus
	textEdit->setFocus();

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
	const QSignalBlocker blocker(textEdit->document());

	//Open dialog window for colors
	QColor col = QColorDialog::getColor(textEdit->textColor(), this);

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
	const QSignalBlocker blocker(textEdit->document());

	//Sets alignment according to what button is pressed
	if (a == actionAlignLeft)
		textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	else if (a == actionAlignCenter)
		textEdit->setAlignment(Qt::AlignHCenter);
	else if (a == actionAlignRight)
		textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	else if (a == actionAlignJustify)
		textEdit->setAlignment(Qt::AlignJustify);

	QTextCursor cursor = textEdit->textCursor();
	//Sends new block format to server
	emit blockFormatChanged(cursor.selectionStart(), cursor.selectionEnd(), cursor.blockFormat());
}


//Merge new format to current char format also for a selection (if present) 
void TextEdit::mergeFormatOnSelection(const QTextCharFormat& format)
{
	//Getting document cursor
	QTextCursor cursor = textEdit->textCursor();

	//Apply format to the document, if the cursor (textEdit->textCursor()) has a selection, apply format to the selection
	textEdit->mergeCurrentCharFormat(format);

	//Sends new char format to server (in case of selection it sends the updated format of every character because they can be different
	for (int i = cursor.selectionStart(); i < cursor.selectionEnd(); ++i) {
		_extraCursor->setPosition(i + 1);
		emit symbolFormatChanged(i, _extraCursor->charFormat());
	}
}


void TextEdit::applyCharFormat(int position, QTextCharFormat fmt)
{
	const QSignalBlocker blocker(textEdit->document());

	_extraCursor->setPosition(position);
	_extraCursor->setPosition(position + 1, QTextCursor::KeepAnchor);

	//Apply char format to text
	_extraCursor->setCharFormat(fmt);

	//Update GUI buttons according to new format
	currentCharFormatChanged(textEdit->textCursor().charFormat());
}


void TextEdit::setLineHeight(QAction* a)
{
	const QSignalBlocker blocker(textEdit->document());

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
	QTextCursor cursor = textEdit->textCursor();
	cursor.mergeBlockFormat(fmt);

	//Sends new block format to server
	emit blockFormatChanged(cursor.selectionStart(), cursor.selectionEnd(), cursor.blockFormat());
}

/**************************** GUI UPDATE ****************************/
/*
*	If there's a change in current format it updates buttons/comboboxes according to new format
*/

//Updates GUI if cursor changes it's position
void TextEdit::cursorPositionChanged()
{
	//Users cursors
	redrawAllCursors();

	//User text higlighting
	updateUsersSelections();

	//Alignment
	alignmentChanged(textEdit->alignment());

	//Line height
	lineHeightChanged(textEdit->textCursor().blockFormat().lineHeight());

	//Lists
	QTextList* list = textEdit->textCursor().currentList();

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
	actionTextStrikethrough->setChecked(f.strikeOut());
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
	if (height == 100)
		actionLineHeight100->setChecked(true);
	else if (height == 115)
		actionLineHeight115->setChecked(true);
	else if (height == 150)
		actionLineHeight150->setChecked(true);
	else if (height == 200)
		actionLineHeight200->setChecked(true);
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
		if (md->hasImage())
			QApplication::clipboard()->clear();
		else
			actionPaste->setEnabled(md->hasText());
}

#endif



/**************************** CONTENTS CHANGE ****************************/
/*
*	This function handles the character insertion/deletion of the document.
*	It's also intercepts all list/block modifications, copy/paste actions, and sets the correct formats.
*	Handle new char from server and delete char from server
*/

void TextEdit::contentsChange(int position, int charsRemoved, int charsAdded)
{
	//Handle character deletion
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
		QChar ch = textEdit->document()->characterAt(i);

		//Emit signal to DocumentEditor to insert a character if is not a null character '\0'
		if (ch != QChar::Null) {
			//Boolean flag to check id it's last '8233' of the document (eof)
			bool isLast = (ch == QChar::ParagraphSeparator) && (i == textEdit->document()->characterCount() - 1);
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
					else emit createNewList(currentBlock.position(), textList->format());
				}

				//Else assign current block to his proper list
				else
					emit assignBlockToList(currentBlock.position(), firstListBlock.position());
			}
			else emit setBlockNoList(currentBlock.position());
		}
	}

	//If in my insertion there's not a 8233 (\n), i apply the correct block format
	if (charsAdded > 1) {
		emit blockFormatChanged(position + charsAdded - 1, position + charsAdded - 1, blockFmt);
	}
}


void TextEdit::newChar(QChar ch, QTextCharFormat format, int position)
{
	const QSignalBlocker blocker(textEdit->document());

	_extraCursor->setPosition(position);

	//Set new char format
	_extraCursor->setCharFormat(format);

	//Insert character at position
	_extraCursor->insertText(ch);
}

void TextEdit::removeChar(int position)
{
	const QSignalBlocker blocker(textEdit->document());

	_extraCursor->setPosition(position);

	//Delete character
	_extraCursor->deleteChar();
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

	if (position < textEdit->document()->characterCount()) {
		//Change user's cursor position
		p->cursor()->setPosition(position);
	}
	else {
		p->cursor()->setPosition(textEdit->document()->characterCount() - 1);
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
	const QRect qRect = textEdit->cursorRect(*cursor);

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

	handleMultipleSelections();
}

//Sets and save all extra selections in the document
void TextEdit::setExtraSelections(qint32 userId, QPair<int, int> selection)
{
	if (onlineUsers.contains(userId)) {
		Presence* p = onlineUsers.find(userId).value();
		QTextCursor cursor(textEdit->document());

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
		p->addUserText(userText);
	}
}

//Sets extra selection in the editor based on what user is checked
void TextEdit::handleMultipleSelections()
{
	//Clear all extra selections
	QList<QTextEdit::ExtraSelection> usersSelections;
	textEdit->setExtraSelections(usersSelections);

	QMap<qint32, Presence*>::iterator it;

	//To check if all actions are checked
	int actionsChecked = 0;

	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		Presence* p = it.value();

		if (p->actionHighlightText()->isChecked()) {
			//Append to list current Presence selection
			usersSelections.append(p->userText());
			actionsChecked++;
		}
	}

	//Check/Uncheck of actionHighlightUsers based on single highlight checked
	if (actionsChecked == 0) {
		actionHighlightUsers->setChecked(false);
	}

	if (actionsChecked == onlineUsers.size()) {
		actionHighlightUsers->setChecked(true);
	}

	//Sets the formatted selections in the editor
	textEdit->setExtraSelections(usersSelections);
}


void TextEdit::updateUsersSelections()
{
	//Clear all selections and recompute new ones
	QMap<qint32, Presence*>::iterator it;
	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		it.value()->clearSelections();
	}

	emit generateExtraSelection();

	handleMultipleSelections();
}


/**************************** DEBUG ****************************/
/*
*	Print document Text
*/

void TextEdit::printDocumenText()
{
	for (int i = 0; i < textEdit->document()->characterCount(); i++) {
		qDebug() << textEdit->document()->characterAt(i);
	}
}

