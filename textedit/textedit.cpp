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
#include <QAbstractTextDocumentLayout>
#include <QToolButton>
#include <QDateTime>

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
#include "ShareUriWindow.h"

const QString rsrcPath = ":/images/win";

TextEdit::TextEdit(QWidget* parent) : QMainWindow(parent), timerId(-1)
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

	//Initialize copy/cut/paste actions
	actionCut->setEnabled(false);
	actionCopy->setEnabled(false);
	actionPaste->setEnabled(false);

	//Makes keyboard focused on text editor
	textEdit->setFocus();

	//Setup cursor position
	_currentCursorPosition = -1;
}

/*
	Queste funzioni generano il menu di File Modifica ...
*/
void TextEdit::setupFileActions()
{
	//Creo una toolbar
	QToolBar* tb = addToolBar(tr("File Actions"));

	//Creo il menu File
	QMenu* menu = menuBar()->addMenu(tr("&File"));


#ifndef QT_NO_PRINTER
	const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));
	QAction* a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(Qt::CTRL + Qt::Key_D);
	tb->addAction(a);

	const QIcon filePrintIcon = QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png"));
	menu->addAction(filePrintIcon, tr("Print Preview..."), this, &TextEdit::filePrintPreview);

	const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));
	a = menu->addAction(printIcon, tr("&Print..."), this, &TextEdit::filePrint);
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

	const QIcon shareIcon = QIcon::fromTheme("share", QIcon(rsrcPath + "/share.png"));
	actionShare = menu->addAction(shareIcon, tr("&Share URI"), this, &TextEdit::fileShare);
	tb->addAction(actionShare);
}

void TextEdit::setupUserActions()
{
	QToolBar* tb = addToolBar(tr("&Account"));
	QMenu* menu = menuBar()->addMenu(tr("&Account"));

	const QIcon userIcon(rsrcPath + "/user.png");
	actionUser = menu->addAction(userIcon, tr("&Edit profile"), this, &TextEdit::openEditProfile);
	tb->addAction(actionUser);


	const QIcon closeDocumentIcon(rsrcPath + "/logout.png");
	actioncloseDocument = menu->addAction(closeDocumentIcon, tr("&Close Document"), this, &TextEdit::askBeforeCloseDocument);
	tb->addAction(actioncloseDocument);

	onlineUsersToolbar = new QToolBar(tr("&Online users"));
	addToolBar(Qt::RightToolBarArea, onlineUsersToolbar);
}

void TextEdit::setupOnlineUsersActions()
{
	QMap<qint32, Presence*>::iterator it;

	onlineUsersToolbar->clear();

	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		Presence* p = it.value();

		QPixmap background(32, 32);

		QColor color = p->color();
		background.fill(color);

		QPainter painter(&background);
		QPixmap userIcon(p->profilePicture());

		painter.drawPixmap(3, 3, 26, 26, userIcon.scaled(26, 26, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		painter.end();

		const QIcon users(background);

		QAction* onlineAction = new QAction(users, p->name().toStdString().c_str(), this);

		connect(onlineAction, &QAction::triggered, this, &TextEdit::handleMultipleSelections);
		onlineAction->setCheckable(true);
		onlineUsersToolbar->addAction(onlineAction);

		p->setAction(onlineAction);
	}
}

void TextEdit::askBeforeCloseDocument()
{
	QMessageBox::StandardButton reply = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("Do you want to close this document?"), QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		killTimer(timerId);
		emit closeDocument();
	}
}



void TextEdit::setupEditActions()
{
	QToolBar* tb = addToolBar(tr("Edit Actions"));
	QMenu* menu = menuBar()->addMenu(tr("&Edit"));

	const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
	actionUndo = menu->addAction(undoIcon, tr("&Undo"), textEdit, &QTextEdit::undo);
	actionUndo->setShortcut(QKeySequence::Undo);
	tb->addAction(actionUndo);

	const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
	actionRedo = menu->addAction(redoIcon, tr("&Redo"), textEdit, &QTextEdit::redo);
	actionRedo->setPriority(QAction::LowPriority);
	actionRedo->setShortcut(QKeySequence::Redo);
	tb->addAction(actionRedo);
	menu->addSeparator();


#ifndef QT_NO_CLIPBOARD
	const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
	actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &QTextEdit::cut);
	actionCut->setPriority(QAction::LowPriority);
	actionCut->setShortcut(QKeySequence::Cut);
	tb->addAction(actionCut);

	const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
	actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &QTextEdit::copy);
	actionCopy->setPriority(QAction::LowPriority);
	actionCopy->setShortcut(QKeySequence::Copy);
	tb->addAction(actionCopy);

	const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
	actionPaste = menu->addAction(pasteIcon, tr("&Paste"), textEdit, &QTextEdit::paste);
	actionPaste->setPriority(QAction::LowPriority);
	actionPaste->setShortcut(QKeySequence::Paste);
	tb->addAction(actionPaste);

	//Controlla se ci sono appunti e abilita il paste solo se ha del testo copiato
	if (const QMimeData* md = QApplication::clipboard()->mimeData())
		actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::setupTextActions()
{
	QToolBar* tb = addToolBar(tr("Format Actions"));
	QMenu* menu = menuBar()->addMenu(tr("F&ormat"));



	const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));
	actionTextBold = menu->addAction(boldIcon, tr("&Bold"), this, &TextEdit::textBold);
	actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
	actionTextBold->setPriority(QAction::LowPriority);

	QFont bold;
	bold.setBold(true);
	actionTextBold->setFont(bold); //Modifico il carattere del menu in Bold
	tb->addAction(actionTextBold);
	actionTextBold->setCheckable(true); //Checkable indica on/off quindi bold/non-bold

	const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));
	actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);
	actionTextItalic->setPriority(QAction::LowPriority);
	actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
	QFont italic;
	italic.setItalic(true);
	actionTextItalic->setFont(italic);
	tb->addAction(actionTextItalic);
	actionTextItalic->setCheckable(true);

	const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));
	actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);
	actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
	actionTextUnderline->setPriority(QAction::LowPriority);
	QFont underline;
	underline.setUnderline(true);
	actionTextUnderline->setFont(underline);
	tb->addAction(actionTextUnderline);
	actionTextUnderline->setCheckable(true);

	menu->addSeparator();
	tb->addSeparator();

	const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
	actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
	actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
	actionAlignLeft->setCheckable(true);
	actionAlignLeft->setPriority(QAction::LowPriority);

	const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
	actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
	actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
	actionAlignCenter->setCheckable(true);
	actionAlignCenter->setPriority(QAction::LowPriority);

	const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
	actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
	actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
	actionAlignRight->setCheckable(true);
	actionAlignRight->setPriority(QAction::LowPriority);

	const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));
	actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);
	actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
	actionAlignJustify->setCheckable(true);
	actionAlignJustify->setPriority(QAction::LowPriority);

	// Make sure the alignLeft is always left of the alignRight
	QActionGroup* alignGroup = new QActionGroup(this);
	connect(alignGroup, &QActionGroup::triggered, this, &TextEdit::textAlign);

	//Se sei un arabo che hai il testo al contrario l'ordine dei pulsanti di allineamento è al contrario
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

	//Per l'allineamento si aggiunge l'intero gruppo al menu, per gestire ordinamento
	tb->addActions(alignGroup->actions());
	menu->addActions(alignGroup->actions());

	//Lists
	tb->addSeparator();

	QMenu* menuList = new QMenu("List menu");
	listStandard = menuList->addAction(tr("Standard"), this, [this]() { listStyle(standard); });
	listStandard->setCheckable(true);
	listStandard->setChecked(true);

	listDisc = menuList->addAction(QIcon(rsrcPath + "/disc.png"), tr("Bullet List - Disc"), this, [this]() { listStyle(disc); });
	listDisc->setCheckable(true);
	listDisc->setChecked(false);

	listCircle = menuList->addAction(QIcon(rsrcPath + "/circle.png"), tr("Bullet List - Circle"), this, [this]() { listStyle(circle); });
	listCircle->setCheckable(true);
	listCircle->setChecked(false);

	listSquare = menuList->addAction(QIcon(rsrcPath + "/square.png"), tr("Bullet List - Square"), this, [this]() { listStyle(square); });
	listSquare->setCheckable(true);
	listSquare->setChecked(false);

	listDecimal = menuList->addAction(QIcon(rsrcPath + "/decimal.png"), tr("Ordered List - Decimal"), this, [this]() { listStyle(decimal); });
	listDecimal->setCheckable(true);
	listDecimal->setChecked(false);

	listAlpha = menuList->addAction(QIcon(rsrcPath + "/alpha.png"), tr("Ordered List - Alpha"), this, [this]() { listStyle(alpha); });
	listAlpha->setCheckable(true);
	listAlpha->setChecked(false);

	listAlphaUpper = menuList->addAction(QIcon(rsrcPath + "/alphaupper.png"), tr("Ordered List - Uppercase alpha"), this, [this]() { listStyle(alphaupper); });
	listAlphaUpper->setCheckable(true);
	listAlphaUpper->setChecked(false);

	listRoman = menuList->addAction(QIcon(rsrcPath + "/roman.png"), tr("Ordered List - Roman"), this, [this]() { listStyle(roman); });
	listRoman->setCheckable(true);
	listRoman->setChecked(false);

	listRomanUpper = menuList->addAction(QIcon(rsrcPath + "/romanupper.png"), tr("Ordered List - Uppercase roman"), this, [this]() { listStyle(romanupper); });
	listRomanUpper->setCheckable(true);
	listRomanUpper->setChecked(false);


	listButton = new QToolButton();
	listButton->setMenu(menuList);
	listButton->setPopupMode(QToolButton::MenuButtonPopup);
	listButton->setDefaultAction(listDisc);

	listButton->setCheckable(true);

	listButton->setIcon(QIcon(rsrcPath + "/list.png"));
	tb->addWidget(listButton);


	//Gestione colore del testo
	menu->addSeparator();
	tb->addSeparator();

	//QPixMap crea l'icona colorata a seconda del colore che selezioniamo
	QPixmap pix(rsrcPath + "/textcolor.png");
	actionTextColor = menu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);
	tb->addAction(actionTextColor);

	//Evidenzia con colore diverso il testo inserito da altri utenti
	menu->addSeparator();
	tb->addSeparator();

	const QIcon HighlightUsersIcon(rsrcPath + "/highlightusers.png");
	actionHighlightUsers = menu->addAction(HighlightUsersIcon, tr("&Highlight users text"), this, &TextEdit::highlightUsersText);
	tb->addAction(actionHighlightUsers);
	//Checkable
	actionHighlightUsers->setCheckable(true);


	//Aggiungo la toolbar all'editor-
	tb = addToolBar(tr("Font and Size"));
	tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBarBreak(Qt::TopToolBarArea);
	addToolBar(tb);


	//Formattazione carattere
	comboFont = new QFontComboBox(tb);
	tb->addWidget(comboFont);
	connect(comboFont, QOverload<const QString&>::of(&QComboBox::activated), this, &TextEdit::textFamily);

	//Dimensione
	comboSize = new QComboBox(tb);
	comboSize->setObjectName("comboSize");
	tb->addWidget(comboSize);
	comboSize->setEditable(true); //Permetto di modificare direttamente la dimensione senza scegliere nel combobox

	//Aggiungo le dimensioni standard dei caratteri prese da QFontDatabase
	const QList<int> standardSizes = QFontDatabase::standardSizes();
	foreach(int size, standardSizes)
		comboSize->addItem(QString::number(size));

	//Setta come dimensione di default la dimensione del testo appena viene caricata QApplication
	comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

	connect(comboSize, QOverload<const QString&>::of(&QComboBox::activated), this, &TextEdit::textSize);
}


void TextEdit::setUser(User* user)
{
	_user = user;

	newPresence(_user->getUserId(), _user->getUsername(), _user->getIcon());
}

void TextEdit::closeDocumentError(QString error)
{
	statusBar()->showMessage(error);
}

//Apply changes to blocks
void TextEdit::applyBlockFormat(int position, QTextBlockFormat fmt)
{
	//const QSignalBlocker blocker(textEdit->document());
	disconnect(textEdit->document(), &QTextDocument::contentsChange, this, &TextEdit::contentsChange);

	_extraCursor->beginEditBlock();

	_extraCursor->setPosition(position);
	_extraCursor->setBlockFormat(fmt);

	_extraCursor->endEditBlock();

	alignmentChanged(fmt.alignment());


	connect(textEdit->document(), &QTextDocument::contentsChange, this, &TextEdit::contentsChange);
}

void TextEdit::criticalError(QString error)
{
	QMessageBox::StandardButton msgbox = QMessageBox::critical(this, QCoreApplication::applicationName(), error, QMessageBox::Ok);
}

void TextEdit::resetUndoRedo()
{
	textEdit->document()->clearUndoRedoStacks();
}


void TextEdit::setDocumentURI(QString uri)
{
	URI = uri;
}

void TextEdit::startCursorTimer()
{
	timerId = startTimer(CURSOR_SEND_INTERVAL);
	qDebug() << "Started timer with ID = " << timerId;
}

void TextEdit::setCurrentFileName(const QString& fileName)
{
	this->fileName = fileName;

	//Sulla finestra appare nomeFile - nomeApplicazione
	setWindowTitle(tr("%1 - %2").arg(fileName, QCoreApplication::applicationName()));
}


void TextEdit::closeEditor()
{
	const QSignalBlocker blocker(textEdit->document());

	onlineUsers.clear();
	textEdit->document()->clear();
	if (timerId > 0)
		killTimer(timerId);
	this->close();

}


//Slot to add a Presence in the editor
void TextEdit::newPresence(qint32 userId, QString username, QImage image)
{
	// Initialize random sequence
	//qsrand(QDateTime::currentMSecsSinceEpoch()*3);

	//Test with user ID for more separate colors
	int randomNumber = 7 + (userId) % 11;

	//Choose a random color from Qt colors
	QColor color = (Qt::GlobalColor) (randomNumber);
	QPixmap userPic;

	userPic.convertFromImage(image);

	if (onlineUsers.contains(userId)) {
		removePresence(userId);
	}

	onlineUsers.insert(userId, new Presence(username, color, userPic, textEdit));
	setupOnlineUsersActions();

	_currentCursorPosition = -1;

	updateUsersSelections();
}

//Remove presence in document
void TextEdit::removePresence(qint32 userId)
{
	Presence* p = onlineUsers.find(userId).value();

	p->label()->clear();
	onlineUsers.remove(userId);
	setupOnlineUsersActions();

	delete p;
	p = nullptr;
}

void TextEdit::filePrint()
{
#if QT_CONFIG(printdialog)
	//Crea oggetto classe stampante che permette di stampare
	QPrinter printer(QPrinter::HighResolution);

	//Apre dialogo di stampa
	QPrintDialog* dlg = new QPrintDialog(&printer, this);

	//Se il cursore ha una selezione permette di stampare solo la selezione (?)
	if (textEdit->textCursor().hasSelection())
		dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);

	//Apre la finestra di stampa con "Print Document"
	dlg->setWindowTitle(tr("Print Document"));
	if (dlg->exec() == QDialog::Accepted)
		//Stampa se accetta
		textEdit->print(&printer);
	delete dlg;
#endif
}

void TextEdit::filePrintPreview()
{
#if QT_CONFIG(printpreviewdialog)
	QPrinter printer(QPrinter::HighResolution);
	//Crea finestra dialogo di anteprima di stampa
	QPrintPreviewDialog preview(&printer, this);
	//Se chiamo stampa dall'anteprima chiama print preview
	connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TextEdit::printPreview);
	//Apre finestra anteprima di stampa
	preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter* printer)
{
#ifdef QT_NO_PRINTER
	Q_UNUSED(printer);
#else
	//Stampa direttamente
	textEdit->print(printer);
#endif
}

void TextEdit::filePrintPdf()
{
#ifndef QT_NO_PRINTER
	//Apre finestra dialogo come Save As... ma con titolo Export PDF
	QFileDialog fileDialog(this, tr("Export PDF"));
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);

	//Mostra solo file PDF
	fileDialog.setMimeTypeFilters(QStringList("application/pdf"));

	fileDialog.setDefaultSuffix("pdf");
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	QString fileName = fileDialog.selectedFiles().first();

	//Richiama classe di stampa, ma forza output come PdfFormat e con il nome fie scelto dalla finestra
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);

	//Stampa direttamente il documento
	textEdit->document()->print(&printer);

	//Mostra nella status bar "Exported PATH"
	statusBar()->showMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(fileName)));
#endif
}

void TextEdit::fileShare()
{
	ShareUriWindow* su = new ShareUriWindow(URI, this);
	//Mostra la finestra di mw formata
	su->exec();
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

	//Formato lista
	QTextListFormat listFmt;

	//Prendo il cursore
	QTextCursor cursor = textEdit->textCursor();

	//Salva il formato del blocco		
	QTextBlockFormat blockFmt = cursor.blockFormat();

	//Creo un oggetto stile carattere come undefined (Standard)
	QTextListFormat::Style style = QTextListFormat::ListStyleUndefined;

	//A seconda del combobox sovrascrivo stile
	switch (styleIndex) {
	case standard:
		listStandard->setChecked(true);
		listDisc->setChecked(false);
		listCircle->setChecked(false);
		listSquare->setChecked(false);
		listDecimal->setChecked(false);
		listAlpha->setChecked(false);
		listAlphaUpper->setChecked(false);
		listRoman->setChecked(false);
		listRomanUpper->setChecked(false);

		listButton->setChecked(false);
		listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		break;

	case disc:
		if (!listDisc->isChecked()) {
			listButton->setChecked(false);
			listDisc->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(true);
			listCircle->setChecked(false);
			listSquare->setChecked(false);
			listDecimal->setChecked(false);
			listAlpha->setChecked(false);
			listAlphaUpper->setChecked(false);
			listRoman->setChecked(false);
			listRomanUpper->setChecked(false);

			listButton->setChecked(true);
			style = QTextListFormat::ListDisc;
			listButton->setIcon(QIcon(rsrcPath + "/disc.png"));
		};
		break;
	case circle:
		if (!listCircle->isChecked()) {
			listButton->setChecked(false);
			listCircle->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(false);
			listCircle->setChecked(true);
			listSquare->setChecked(false);
			listDecimal->setChecked(false);
			listAlpha->setChecked(false);
			listAlphaUpper->setChecked(false);
			listRoman->setChecked(false);
			listRomanUpper->setChecked(false);

			listButton->setChecked(true);
			style = QTextListFormat::ListCircle;
			listButton->setIcon(QIcon(rsrcPath + "/circle.png"));
		};
		break;
	case square:
		if (!listSquare->isChecked()) {
			listButton->setChecked(false);
			listSquare->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(false);
			listCircle->setChecked(false);
			listSquare->setChecked(true);
			listDecimal->setChecked(false);
			listAlpha->setChecked(false);
			listAlphaUpper->setChecked(false);
			listRoman->setChecked(false);
			listRomanUpper->setChecked(false);

			listButton->setChecked(true);
			style = QTextListFormat::ListSquare;
			listButton->setIcon(QIcon(rsrcPath + "/square.png"));
		};
		break;
	case decimal:
		if (!listDecimal->isChecked()) {
			listButton->setChecked(false);
			listDecimal->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(false);
			listCircle->setChecked(false);
			listSquare->setChecked(false);
			listDecimal->setChecked(true);
			listAlpha->setChecked(false);
			listAlphaUpper->setChecked(false);
			listRoman->setChecked(false);
			listRomanUpper->setChecked(false);

			listButton->setChecked(true);
			style = QTextListFormat::ListDecimal;
			listButton->setIcon(QIcon(rsrcPath + "/decimal.png"));
		};
		break;

	case alpha:
		if (!listAlpha->isChecked()) {
			listButton->setChecked(false);
			listAlpha->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(false);
			listCircle->setChecked(false);
			listSquare->setChecked(false);
			listDecimal->setChecked(false);
			listAlpha->setChecked(true);
			listAlphaUpper->setChecked(false);
			listRoman->setChecked(false);
			listRomanUpper->setChecked(false);

			listButton->setChecked(true);
			style = QTextListFormat::ListLowerAlpha;
			listButton->setIcon(QIcon(rsrcPath + "/alpha.png"));
		};
		break;

	case alphaupper:
		if (!listAlphaUpper->isChecked()) {
			listButton->setChecked(false);
			listAlphaUpper->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(false);
			listCircle->setChecked(false);
			listSquare->setChecked(false);
			listDecimal->setChecked(false);
			listAlpha->setChecked(false);
			listAlphaUpper->setChecked(true);
			listRoman->setChecked(false);
			listRomanUpper->setChecked(false);

			listButton->setChecked(true);
			style = QTextListFormat::ListUpperAlpha;
			listButton->setIcon(QIcon(rsrcPath + "/alphaupper.png"));
		};
		break;

	case roman:
		if (!listRoman->isChecked()) {
			listButton->setChecked(false);
			listRoman->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(false);
			listCircle->setChecked(false);
			listSquare->setChecked(false);
			listDecimal->setChecked(false);
			listAlpha->setChecked(false);
			listAlphaUpper->setChecked(false);
			listRoman->setChecked(true);
			listRomanUpper->setChecked(false);

			listButton->setChecked(true);
			style = QTextListFormat::ListLowerRoman;
			listButton->setIcon(QIcon(rsrcPath + "/roman.png"));
		};
		break;

	case romanupper:
		if (!listRomanUpper->isChecked()) {
			listButton->setChecked(false);
			listRomanUpper->setChecked(false);
			listStandard->setChecked(true);

			listButton->setIcon(QIcon(rsrcPath + "/list.png"));

		}
		else
		{
			listStandard->setChecked(false);
			listDisc->setChecked(false);
			listCircle->setChecked(false);
			listSquare->setChecked(false);
			listDecimal->setChecked(false);
			listAlpha->setChecked(false);
			listAlphaUpper->setChecked(false);
			listRoman->setChecked(false);
			listRomanUpper->setChecked(true);

			listButton->setChecked(true);
			style = QTextListFormat::ListUpperRoman;
			listButton->setIcon(QIcon(rsrcPath + "/romanupper.png"));
		};
		break;
	default:
		break;
	}

	//Set new list style
	listFmt.setStyle(style);

	//Tells to server new list format
	emit toggleList(cursor.selectionStart(), cursor.selectionEnd(), listFmt);

}

void TextEdit::createList(int position, QTextListFormat fmt)
{
	const QSignalBlocker blocker(textEdit->document());

	//Setting list indentation to 1 step
	fmt.setIndent(1);

	//Moving to desired position to create the list
	_extraCursor->setPosition(position);

	//Creating list with given format
	_extraCursor->createList(fmt);
}

void TextEdit::removeBlockFromList(int blockPosition)
{
	const QSignalBlocker blocker(textEdit->document());
	//Moving to target block
	_extraCursor->setPosition(blockPosition);

	//Getting current list
	QTextList* currentList = _extraCursor->currentList();

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

void TextEdit::addBlockToList(int listPosition, int blockPosition)
{
	const QSignalBlocker blocker(textEdit->document());

	//Getting list at listPosition
	_extraCursor->setPosition(listPosition);
	QTextList* currentList = _extraCursor->currentList();

	//Getting block at blockPosition
	_extraCursor->setPosition(blockPosition);
	QTextBlock blk = _extraCursor->block();

	//Add block to list
	currentList->add(blk);
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
		switch (list->format().style()) {
		case QTextListFormat::ListDisc:
			listButton->setChecked(true);
			listDisc->setChecked(true);
			listStandard->setChecked(false);
			listButton->setIcon(QIcon(rsrcPath + "/disc.png"));
			break;
		case QTextListFormat::ListCircle:
			listButton->setChecked(true);
			listCircle->setChecked(true);
			listStandard->setChecked(false);
			listButton->setIcon(QIcon(rsrcPath + "/circle.png"));
			break;
		case QTextListFormat::ListSquare:
			listButton->setChecked(true);
			listSquare->setChecked(true);
			listStandard->setChecked(false);
			listButton->setIcon(QIcon(rsrcPath + "/square.png"));
			break;
		case QTextListFormat::ListDecimal:
			listButton->setChecked(true);
			listDecimal->setChecked(true);
			listButton->setIcon(QIcon(rsrcPath + "/decimal.png"));
			break;
		case QTextListFormat::ListLowerAlpha:
			listButton->setChecked(true);
			listAlpha->setChecked(true);
			listStandard->setChecked(false);
			listButton->setIcon(QIcon(rsrcPath + "/alpha.png"));
			break;
		case QTextListFormat::ListUpperAlpha:
			listButton->setChecked(true);
			listStandard->setChecked(false);
			listAlphaUpper->setChecked(true);
			listButton->setIcon(QIcon(rsrcPath + "/alphaupper.png"));
			break;
		case QTextListFormat::ListLowerRoman:
			listButton->setChecked(true);
			listRoman->setChecked(true);
			listStandard->setChecked(false);
			listButton->setIcon(QIcon(rsrcPath + "/roman.png"));
			break;
		case QTextListFormat::ListUpperRoman:
			listButton->setChecked(true);
			listRomanUpper->setChecked(true);
			listStandard->setChecked(false);
			listButton->setIcon(QIcon(rsrcPath + "/romanupper.png"));
			break;
		default:
			break;
		}
	}
	else {
		listButton->setChecked(false);
		listStandard->setChecked(true);
		listButton->setIcon(QIcon(rsrcPath + "/list.png"));
	}
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

void TextEdit::contentsChange(int position, int charsRemoved, int charsAdded) {

	//Handle character deletion
	for (int i = 0; i < charsRemoved; ++i) {
		emit charDeleted(position);
	}


	QTextBlockFormat blockFmt;

	for (int i = position; i < position + charsAdded; ++i) {
		//Getting QTextBlockFormat from cursor
		blockFmt = _extraCursor->blockFormat();

		//Ricavo il carattere inserito
		QChar ch = textEdit->document()->characterAt(i);

		//Setto il cursore alla posizione+1 perchè il formato (charFormat) viene verificato sul carattere
		//precedente al cursore.
		_extraCursor->setPosition(i + 1);

		//Getting QTextCharFormat from cursor
		QTextCharFormat fmt = _extraCursor->charFormat();

		//Emit signal to DocumentEditor to insert a character
		emit charInserted(ch, fmt, i);

		if (ch == QChar::ParagraphSeparator) {
			emit blockFormatChanged(i, i, blockFmt);

			//Check if current block is in a list
			_extraCursor->setPosition(i);

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

	//Change user's cursor position
	p->cursor()->setPosition(position);

	drawGraphicCursor(p);
}

//Redraw all cursor in case of window update (scroll, resize...)
void TextEdit::redrawAllCursors() {

	QMap<qint32, Presence*>::iterator it;

	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		if (it.key() != _user->getUserId()) {

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


/*	HIGHLIGHT USER'S TEXT SELECTION
*
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

/*	TIMER EVENT
*
*	Handle timer event to send cursor position to the server
*/
void TextEdit::timerEvent(QTimerEvent* event)
{
	//if (textEdit->textCursor().position() != _currentCursorPosition) {
	_currentCursorPosition = textEdit->textCursor().position();
	emit newCursorPosition(textEdit->textCursor().position());
	//}
}
