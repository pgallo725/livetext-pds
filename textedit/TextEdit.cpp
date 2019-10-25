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

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>

#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif

#include <QPrinter>

#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "textedit.h"

const QString rsrcPath = ":/images/win";

TextEdit::TextEdit(User& user, QWidget* parent) : QMainWindow(parent), timerId(-1), _user(user)
{
	//Setting window title
	setWindowTitle(QCoreApplication::applicationName());
	//Setting window icon
	setWindowIcon(QIcon(":/images/logo.png"));

	//Inizialize Qt text editor
	textEdit = new QTextEdit(this);

	//Assign textEdit as the central widget
	setCentralWidget(textEdit);

	/**************************** GUI SETUP ****************************/

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
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::redrawAllCursors);


	//Online users text highlight redraw in case of window aspect, char format, cursor position changed
	connect(textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::updateUsersSelections);

	//Undo/Redo actions binded to document Undo/Redo
	connect(textEdit->document(), &QTextDocument::undoAvailable, actionUndo, &QAction::setEnabled);
	connect(textEdit->document(), &QTextDocument::redoAvailable, actionRedo, &QAction::setEnabled);

	//Mandatory to intercept character insertion, the document emit this signal every time a character inside document is added/removed
	connect(textEdit->document(), &QTextDocument::contentsChange, this, &TextEdit::contentsChange);


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

	//Initialize GUI, activating buttons according to current conditions
	fontChanged(textEdit->font());
	colorChanged(textEdit->textColor());
	alignmentChanged(textEdit->alignment());

	//Initialize Undo/Redo actions
	actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
	actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

#ifndef QT_NO_CLIPBOARD
	//Initialize copy/cut/paste actions
	actionCut->setEnabled(false);
	actionCopy->setEnabled(false);
	actionPaste->setEnabled(false);
#endif

	//Makes keyboard focused on text editor
	textEdit->setFocus();

	//Setup cursor position
	_currentCursorPosition = -1;

	//Generate new presence for current user
	newPresence(_user.getUserId(), _user.getUsername(), _user.getIcon());
}

/**************************** GUI SETUP ****************************/
/*
*	This functions configure all Menu entries
*	File (Export PDF, Print)
*	Share document
*	Edit (Undo, Redo, Cut, Copy, Paste)
*	Text (TextFormat, Lists, Alignment)
*	Account actions (Edit profile, Close document)
*/
void TextEdit::setupFileActions()
{
	//New toolbar
	QToolBar* tb = addToolBar(tr("File Actions"));

	//New menu
	QMenu* menu = menuBar()->addMenu(tr("&File"));

	//If the print plugin is enabled
#ifndef QT_NO_PRINTER

	//Export document in PDF
	const QIcon exportPdfIcon = QIcon(rsrcPath + "/exportpdf.png");
	QAction* a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(Qt::CTRL + Qt::Key_D);
	tb->addAction(a);

	//Print document
	const QIcon filePrintIcon = QIcon(rsrcPath + "/fileprint.png");
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
	const QIcon shareIcon = QIcon(rsrcPath + "/share.png");
	QAction* a = menu->addAction(shareIcon, tr("&Share URI"), this, &TextEdit::fileShare);
	tb->addAction(a);
}


void TextEdit::setupEditActions()
{
	QToolBar* tb = addToolBar(tr("Edit Actions"));
	QMenu* menu = menuBar()->addMenu(tr("&Edit"));

	//Undo
	const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
	actionUndo = menu->addAction(undoIcon, tr("&Undo"), textEdit, &QTextEdit::undo);
	actionUndo->setShortcut(QKeySequence::Undo);
	tb->addAction(actionUndo);

	//Redo
	const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
	actionRedo = menu->addAction(redoIcon, tr("&Redo"), textEdit, &QTextEdit::redo);
	actionRedo->setPriority(QAction::LowPriority);
	actionRedo->setShortcut(QKeySequence::Redo);
	tb->addAction(actionRedo);

	menu->addSeparator();


#ifndef QT_NO_CLIPBOARD
	//Cut
	const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
	actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &QTextEdit::cut);
	actionCut->setPriority(QAction::LowPriority);
	actionCut->setShortcut(QKeySequence::Cut);
	tb->addAction(actionCut);

	//Copy
	const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
	actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &QTextEdit::copy);
	actionCopy->setPriority(QAction::LowPriority);
	actionCopy->setShortcut(QKeySequence::Copy);
	tb->addAction(actionCopy);

	//Paste
	const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
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
	const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));
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
	const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));
	actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);
	actionTextItalic->setPriority(QAction::LowPriority);
	actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
	actionTextItalic->setCheckable(true);

	QFont italic;
	italic.setItalic(true);
	actionTextItalic->setFont(italic);

	tb->addAction(actionTextItalic);


	//Underline
	const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));
	actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);
	actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
	actionTextUnderline->setPriority(QAction::LowPriority);
	actionTextUnderline->setCheckable(true);

	QFont underline;
	underline.setUnderline(true);
	actionTextUnderline->setFont(underline);

	tb->addAction(actionTextUnderline);

	menu->addSeparator();
	tb->addSeparator();


	//Alignment

	//Left
	const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
	actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
	actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
	actionAlignLeft->setCheckable(true);
	actionAlignLeft->setPriority(QAction::LowPriority);

	//Center
	const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
	actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
	actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
	actionAlignCenter->setCheckable(true);
	actionAlignCenter->setPriority(QAction::LowPriority);

	//Right
	const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
	actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
	actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
	actionAlignRight->setCheckable(true);
	actionAlignRight->setPriority(QAction::LowPriority);

	//Justify
	const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));
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

	listActions[disc] = menuList->addAction(QIcon(rsrcPath + "/disc.png"), tr("Bullet List - Disc"), this, [this]() { listStyle(disc); });
	listActions[disc]->setCheckable(true);
	listActions[disc]->setChecked(false);

	listActions[circle] = menuList->addAction(QIcon(rsrcPath + "/circle.png"), tr("Bullet List - Circle"), this, [this]() { listStyle(circle); });
	listActions[circle]->setCheckable(true);
	listActions[circle]->setChecked(false);

	listActions[square] = menuList->addAction(QIcon(rsrcPath + "/square.png"), tr("Bullet List - Square"), this, [this]() { listStyle(square); });
	listActions[square]->setCheckable(true);
	listActions[square]->setChecked(false);

	listActions[decimal] = menuList->addAction(QIcon(rsrcPath + "/decimal.png"), tr("Ordered List - Decimal"), this, [this]() { listStyle(decimal); });
	listActions[decimal]->setCheckable(true);
	listActions[decimal]->setChecked(false);

	listActions[alpha] = menuList->addAction(QIcon(rsrcPath + "/alpha.png"), tr("Ordered List - Alpha"), this, [this]() { listStyle(alpha); });
	listActions[alpha]->setCheckable(true);
	listActions[alpha]->setChecked(false);

	listActions[alphaupper] = menuList->addAction(QIcon(rsrcPath + "/alphaupper.png"), tr("Ordered List - Uppercase alpha"), this, [this]() { listStyle(alphaupper); });
	listActions[alphaupper]->setCheckable(true);
	listActions[alphaupper]->setChecked(false);

	listActions[roman] = menuList->addAction(QIcon(rsrcPath + "/roman.png"), tr("Ordered List - Roman"), this, [this]() { listStyle(roman); });
	listActions[roman]->setCheckable(true);
	listActions[roman]->setChecked(false);

	listActions[romanupper] = menuList->addAction(QIcon(rsrcPath + "/romanupper.png"), tr("Ordered List - Uppercase roman"), this, [this]() { listStyle(romanupper); });
	listActions[romanupper]->setCheckable(true);
	listActions[romanupper]->setChecked(false);


	//Setup ToolButton
	listButton = new QToolButton();
	listButton->setMenu(menuList);
	listButton->setPopupMode(QToolButton::MenuButtonPopup);
	listButton->setDefaultAction(listActions[disc]);

	listButton->setCheckable(true);

	listButton->setIcon(QIcon(rsrcPath + "/list.png"));
	tb->addWidget(listButton);

	menu->addSeparator();
	tb->addSeparator();

	//Color
	QPixmap pix(rsrcPath + "/textcolor.png");
	actionTextColor = menu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);
	tb->addAction(actionTextColor);

	menu->addSeparator();
	tb->addSeparator();

	//Highlight user text
	const QIcon HighlightUsersIcon(rsrcPath + "/highlightusers.png");
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
	comboSize->setEditable(true); //Permetto di modificare direttamente la dimensione senza scegliere nel combobox

	//Adding standard sizes to combobox
	const QList<int> standardSizes = QFontDatabase::standardSizes();
	foreach(int size, standardSizes)
		comboSize->addItem(QString::number(size));

	//Update current index according to format
	comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

	connect(comboSize, QOverload<const QString&>::of(&QComboBox::activated), this, &TextEdit::textSize);
}

void TextEdit::setupUserActions()
{
	QToolBar* tb = addToolBar(tr("&Account"));
	QMenu* menu = menuBar()->addMenu(tr("&Account"));

	//Edit profile
	const QIcon userIcon(rsrcPath + "/user.png");
	QAction* a = menu->addAction(userIcon, tr("&Edit profile"), this, &TextEdit::openEditProfile);
	tb->addAction(a);

	//Close document
	const QIcon closeDocumentIcon(rsrcPath + "/logout.png");
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
*	Close document msgbox
*	Close editor
*	Error messages
*	Reset Undo/Redo
*	Start timer
*	Setting document filename, URI
*	File print, Print Preview
*	Save as PDF
*	Share document URI
*/

void TextEdit::askBeforeCloseDocument()
{
	QMessageBox::StandardButton reply = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("Do you want to close this document?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		killTimer(timerId);
		emit closeDocument();
	}
}


void TextEdit::closeDocumentError(QString error)
{
	statusBar()->showMessage(error);
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


void TextEdit::resetUndoRedo()
{
	textEdit->document()->clearUndoRedoStacks();
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
	emit newCursorPosition(textEdit->textCursor().position());
	//}
}


void TextEdit::setCurrentFileName(QString fileName, QString uri)
{
	this->fileName = fileName;
	this->URI = uri;

	_shareUri = new ShareUriWindow(URI, statusBar());

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
	statusBar()->showMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(fileName)));
#endif
}

void TextEdit::fileShare()
{
	//Show created window
	_shareUri->exec();
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

	if (currentList) {
		qDebug() << "Removing block at position " << blockPosition;

		//Getting current block
		QTextBlock blk = _extraCursor->block();

		//Getting current block format
		QTextBlockFormat blkFormat = _extraCursor->blockFormat();

		//Remove target bock from list
		currentList->remove(blk);

		//Makes the index of the blockFormat object -1 --> Reset block format to default
		blkFormat.setObjectIndex(-1);

		//Apply new format
		_extraCursor->setBlockFormat(blkFormat);

		//Reload updated block format to send it to the server
		blkFormat = _extraCursor->blockFormat();

		//Sends new block format to server
		emit blockFormatChanged(blockPosition, blockPosition, blkFormat);

	}

	cursorPositionChanged();
}

void TextEdit::addBlockToList(int listPosition, int blockPosition)
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

	QTextBlockFormat format;
	format.setAlignment(fmt.alignment());

	//Sets block format in current block
	_extraCursor->mergeBlockFormat(format);

	//Call alignment changed for update GUI
	alignmentChanged(fmt.alignment());
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

	//Casting point size to float
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0) {
		//Set font size
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);

		//Apply format
		mergeFormatOnSelection(fmt);
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



/**************************** GUI UPDATE ****************************/
/*
*	If there's a change in current format it updates buttons/comboboxes according to new format
*/

//Updates GUI if cursor changes it's position
void TextEdit::cursorPositionChanged()
{
	//Alignment
	alignmentChanged(textEdit->alignment());

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
}

void TextEdit::colorChanged(const QColor& c)
{
	//Change color of textColor button according to text
	QPixmap pix(rsrcPath + "/textcolor.png");
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

/**************************** PASTE EVENT ****************************/
/*
*	When clipboard content changes, it prevents pasting images
*/

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
	//Se ho del testo negli appunti allora si sblocca il pulsante incolla
	if (const QMimeData* md = QApplication::clipboard()->mimeData()) {
		actionPaste->setEnabled(md->hasText());
		actionPaste->setEnabled(!md->hasImage());
	}
#endif
}


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


			//Getting current list (if present)
			QTextList* textList = _extraCursor->currentList();

			if (textList) {
				//Getting current block
				QTextBlock currentBlock = _extraCursor->block();

				//Getting first block of the list
				QTextBlock firstListBlock = textList->item(0);

				//If current block is the beginning of its list emit the signal to create a new list
				if (currentBlock == firstListBlock)
					emit createNewList(currentBlock.position(), textList->format());
				//Else assign current block to his proper list
				else
					emit assignBlockToList(currentBlock.position(), firstListBlock.position());
			}
		}
	}

	//If in my insertion there's not a 8233 (\n), i apply the correct block format
	if (charsAdded > 1) {
		emit blockFormatChanged(position + charsAdded - 1, position + charsAdded - 1, blockFmt);
	}
}


void TextEdit::newChar(QChar ch, QTextCharFormat format, int position, qint32 user)
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

