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
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));

	textEdit = new QTextEdit(this);


	// Permettono di connettere le funzioni di QTextEdit con quelle di TextEdit ovvero:
	// quando viene invocata la funzione currentCharFormatChanged genera un segnale che viene recepito
	// dall' oggetto TextEdit (this) che invoca la funzione (slot) currentCharFormatChanged
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::currentCharFormatChanged);
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::cursorPositionChanged);


	//Gestione automatica della posizione del cursore degli altri utenti quando cambia la visualizzazione
	connect(textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::handleUsersCursors);
	connect(textEdit->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::handleUsersCursors);
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::handleUsersCursors);
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::handleUsersCursors);


	//Gestione automatica della posizione delle selezioni degli altri utenti quando cambia la visualizzazione
	connect(textEdit->verticalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit->horizontalScrollBar(), &QScrollBar::valueChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::updateUsersSelections);
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::updateUsersSelections);


	//Assegna il Widget textEdit alla finestra principaòe
	setCentralWidget(textEdit);
	//Funzioni definite sotto e chiamate nel costruttore
	setupFileActions();
	setupEditActions();
	setupTextActions();
	setupShareActions();
	setupUserActions();

	//Crea il carattere e lo stile
	QFont textFont("Helvetica");
	textFont.setStyleHint(QFont::SansSerif);

	//Setta il carattere nell'editor
	textEdit->setFont(textFont);

	//Le funzioni vengono spiegate sotto
	fontChanged(textEdit->font());
	colorChanged(textEdit->textColor());
	alignmentChanged(textEdit->alignment());


	//Connect delle azioni sul documento QTextDocument alle azioni di QAction rendendole abilitate
	//Esempio: L'azione Save sul documento se ho appena salvato non è disponibile, appena faccio una modificia (modificationChanged) la rendo effettuabile
	connect(textEdit->document(), &QTextDocument::modificationChanged, this, &QWidget::setWindowModified);
	connect(textEdit->document(), &QTextDocument::undoAvailable, actionUndo, &QAction::setEnabled);
	connect(textEdit->document(), &QTextDocument::redoAvailable, actionRedo, &QAction::setEnabled);

	//Stesso discorso delle connect ma inizializza
	setWindowModified(textEdit->document()->isModified());
	actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
	actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

	//Mandatory to intercept character insertion, the document emit this signal every time a character inside document is added/removedè
	connect(textEdit->document(), &QTextDocument::contentsChange, this, &TextEdit::contentsChange);

	//Se non ho gli appunti disponibili
#ifndef QT_NO_CLIPBOARD
	actionCut->setEnabled(false);
	connect(textEdit, &QTextEdit::copyAvailable, actionCut, &QAction::setEnabled);

	actionCopy->setEnabled(false);
	connect(textEdit, &QTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);

	connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEdit::clipboardDataChanged);
#endif

	//Rende la tastiera attiva sul widget
	textEdit->setFocus();

	//Setup cursor position
	_currentCursorPosition = -1;

	//Setup extra cursor
	_extraCursor = new QTextCursor(textEdit->document());
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

	a = menu->addAction(tr("&Quit"), this, &QWidget::close);
	a->setShortcut(Qt::CTRL + Qt::Key_Q);
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

	// Make sure the alignLeft  is always left of the alignRight
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

	//Creo il combobox per lo stile del testo -- Da modificare a piacimento
	comboStyle = new QComboBox(tb);
	tb->addWidget(comboStyle);
	comboStyle->addItem("Standard");
	comboStyle->addItem("Heading 1");
	comboStyle->addItem("Heading 2");
	comboStyle->addItem("Heading 3");
	comboStyle->addItem("Heading 4");
	comboStyle->addItem("Heading 5");
	comboStyle->addItem("Heading 6");

	//Connetto la selezione del combobox con la formattazione di tutto il testo funzione spiegata dopo
	connect(comboStyle, QOverload<int>::of(&QComboBox::activated), this, &TextEdit::textStyle);


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
	const QSignalBlocker blocker(textEdit->document());

	_extraCursor->setPosition(position);
	_extraCursor->mergeBlockFormat(fmt);

	alignmentChanged(fmt.alignment());

	//Setta nel combobox l'heading level corretto
	comboStyle->setCurrentIndex(fmt.headingLevel() ? fmt.headingLevel() : 0);

}

//Applies symbol format
void TextEdit::applyCharFormat(int position, QTextCharFormat fmt)
{
	const QSignalBlocker blocker(textEdit->document());

	_extraCursor->setPosition(position);
	_extraCursor->setPosition(position + 1, QTextCursor::KeepAnchor);
	_extraCursor->mergeCharFormat(fmt);
	textEdit->mergeCurrentCharFormat(fmt);
}

void TextEdit::criticalError(QString error)
{
	QMessageBox::StandardButton msgbox = QMessageBox::critical(this, QCoreApplication::applicationName(), error, QMessageBox::Ok);
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

void TextEdit::newChar(QChar ch, QTextCharFormat format, int position, qint32 user)
{
	const QSignalBlocker blocker(textEdit->document());

	QTextCursor* cursor;

	if (user != -1) {
		Presence* p = onlineUsers.find(user).value();
		cursor = p->cursor();
	}
	else {
		cursor = new QTextCursor(textEdit->document());
	}

	cursor->setPosition(position);

	cursor->setCharFormat(format);

	cursor->insertText(ch);
	updateUsersSelections();
}

void TextEdit::removeChar(int position)
{
	const QSignalBlocker blocker(textEdit->document());

	QTextCursor* cursor = new QTextCursor(textEdit->document());
	cursor->setPosition(position);

	cursor->deleteChar();
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
	int randomNumber = 7 + (userId * 3) % 11;

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

void TextEdit::textBold()
{
	const QSignalBlocker blocker(textEdit->document());

	QTextCharFormat fmt;

	//Controlla se l'azione TextBold è attivata e imposta il Weight normale/bold
	fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
	const QSignalBlocker blocker(textEdit->document());

	QTextCharFormat fmt;
	fmt.setFontUnderline(actionTextUnderline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
	const QSignalBlocker blocker(textEdit->document());

	QTextCharFormat fmt;
	fmt.setFontItalic(actionTextItalic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

//Tipo di carattere (Arial...)
void TextEdit::textFamily(const QString& f)
{
	const QSignalBlocker blocker(textEdit->document());

	QTextCharFormat fmt;
	fmt.setFontFamily(f);
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString& p)
{
	const QSignalBlocker blocker(textEdit->document());

	//Casta a float (?) e imposta la dimensione del carattere
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void TextEdit::listStyle(int styleIndex)
{
	//const QSignalBlocker blocker(textEdit->document());

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

	//Indica l'inizio dell'editing a cui si appoggi l'undo/redo
	cursor.beginEditBlock();


	if (style == QTextListFormat::ListStyleUndefined) {
		//Se Standard lo stile
		blockFmt.setObjectIndex(-1); //(?)

		//blockFmt.setHeadingLevel(0);
		cursor.setBlockFormat(blockFmt);
	}
	//Indica se il cursore è gia in una lista, se sì ne prende il formato
	else if (cursor.currentList())
	{
		listFmt = cursor.currentList()->format();

		//Metto lo stile scelto nello switch come formato lista
		listFmt.setStyle(style);

		//Creo la lista indentata correttamente
		cursor.createList(listFmt);

	}
	else {
		//Altrimenti se non sono in una lista indento di +1
		listFmt.setIndent(blockFmt.indent() + 1);
		blockFmt.setIndent(0);

		//Setto il formato del blocco
		cursor.setBlockFormat(blockFmt);

		listFmt.setStyle(style);

		cursor.createList(listFmt);
	}
	cursor.endEditBlock();
}

void TextEdit::textStyle(int styleIndex)
{
	//Prendo il cursore
	QTextCursor cursor = textEdit->textCursor();
	int headingLevel = styleIndex > 0 ? styleIndex : 0; // H1 to H6, or Standard
	int sizeAdjustment = headingLevel ? 4 - headingLevel : 0; // H1 to H6: +3 to -2

	const QSignalBlocker blocker(textEdit->document());

	//Crea formattazione carattere
	QTextCharFormat fmt;
	//Se ho degli heading mette grassetto
	fmt.setFontWeight(headingLevel ? QFont::Bold : QFont::Normal);
	//Aggiorna il size da +3 a -2 a seconda dell'Heading
	fmt.setProperty(QTextFormat::FontSizeAdjustment, sizeAdjustment);

	//Indica l'intera linea su cui sta il cursore
	mergeFormatOnWordOrSelection(fmt);


	//Indica l'inizio dell'editing a cui si appoggi l'undo/redo
	cursor.beginEditBlock();

	//Salva il formato del blocco
	QTextBlockFormat blockFmt = cursor.blockFormat();

	//Se Standard lo stile
	blockFmt.setObjectIndex(-1); //(?)

	//Per evitare tutti gli heading mette che l'index del combobox 9 = H1 , 10 = H2...
	blockFmt.setHeadingLevel(headingLevel);
	cursor.setBlockFormat(blockFmt);

	cursor.endEditBlock();

	emit blockFormatChanged(cursor.selectionStart(), cursor.selectionEnd(), cursor.blockFormat());
}

void TextEdit::textColor()
{
	const QSignalBlocker blocker(textEdit->document());

	//Creo finestra di dialogo per colori
	QColor col = QColorDialog::getColor(textEdit->textColor(), this);

	if (!col.isValid()) //Se colore non valido torno
		return;

	QTextCharFormat fmt;
	fmt.setForeground(col);
	mergeFormatOnWordOrSelection(fmt);

	//Se cambia colore cambio icona colore
	colorChanged(col);
}

//Funzione chiamata con il bind di triggered del gruppo di pulsanti di allineamento
//Sono esclusivi
void TextEdit::textAlign(QAction* a)
{
	const QSignalBlocker blocker(textEdit->document());

	//Applico gli allineamenti
	if (a == actionAlignLeft)
		textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	else if (a == actionAlignCenter)
		textEdit->setAlignment(Qt::AlignHCenter);
	else if (a == actionAlignRight)
		textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	else if (a == actionAlignJustify)
		textEdit->setAlignment(Qt::AlignJustify);

	QTextCursor cursor = textEdit->textCursor();

	emit blockFormatChanged(cursor.selectionStart(), cursor.selectionEnd(), cursor.blockFormat());
}

void TextEdit::alignmentChanged(Qt::Alignment a)
{
	//Se cambia allineamento setto i vari checked degli allineamenti
	if (a & Qt::AlignLeft)
		actionAlignLeft->setChecked(true);
	else if (a & Qt::AlignHCenter)
		actionAlignCenter->setChecked(true);
	else if (a & Qt::AlignRight)
		actionAlignRight->setChecked(true);
	else if (a & Qt::AlignJustify)
		actionAlignJustify->setChecked(true);
}

//Bind di QTextEdit quando cambia il formato
void TextEdit::currentCharFormatChanged(const QTextCharFormat& format)
{
	//Applica cambiamenti chiamando gli slot opportuni
	fontChanged(format.font());
	colorChanged(format.foreground().color());
}

//Questa funzione aggiorna i pulsanti e i combobox a seconda della posizione del cursore
void TextEdit::cursorPositionChanged()
{
	//Se cambia allineamento testo applico modifiche ai pulsanti markandoli come checked
	alignmentChanged(textEdit->alignment());

	//Ricava l'elenco puntato in cui si trova il cursore
	//Ne ricava lo stile della lista e lo setta nel pulsante
	QTextList* list = textEdit->textCursor().currentList();
	if (list) {
		switch (list->format().style()) {
		case QTextListFormat::ListDisc:
			listButton->setChecked(true);
			listDisc->setChecked(true);
			listButton->setIcon(QIcon(rsrcPath + "/disc.png"));
			break;
		case QTextListFormat::ListCircle:
			listButton->setChecked(true);
			listCircle->setChecked(true);
			listButton->setIcon(QIcon(rsrcPath + "/circle.png"));
			break;
		case QTextListFormat::ListSquare:
			listButton->setChecked(true);
			listSquare->setChecked(true);
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
			listButton->setIcon(QIcon(rsrcPath + "/alpha.png"));
			break;
		case QTextListFormat::ListUpperAlpha:
			listButton->setChecked(true);
			listAlphaUpper->setChecked(true);
			listButton->setIcon(QIcon(rsrcPath + "/alphaupper.png"));
			break;
		case QTextListFormat::ListLowerRoman:
			listButton->setChecked(true);
			listRoman->setChecked(true);
			listButton->setIcon(QIcon(rsrcPath + "/roman.png"));
			break;
		case QTextListFormat::ListUpperRoman:
			listButton->setChecked(true);
			listRomanUpper->setChecked(true);
			listButton->setIcon(QIcon(rsrcPath + "/romanupper.png"));
			break;
		default:
			break;
		}
	}
	else {
		listButton->setChecked(false);
		listButton->setIcon(QIcon(rsrcPath + "/list.png"));
	}

	//Setta nel combobox l'heading level corretto
	int headingLevel = textEdit->textCursor().blockFormat().headingLevel();
	comboStyle->setCurrentIndex(headingLevel ? headingLevel : 0);
}

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
	//Se ho del testo negli appunti allora si sblocca il pulsante incolla
	if (const QMimeData* md = QApplication::clipboard()->mimeData())
		actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat& format)
{
	//Chiamato quando devo cambiare il formato se ho una selezione
	QTextCursor cursor = textEdit->textCursor();


	//Applico formattazione alla selezione
	cursor.mergeCharFormat(format);
	//Applico formattazione anche al documento
	textEdit->mergeCurrentCharFormat(format);

	for (int i = cursor.selectionStart(); i < cursor.selectionEnd(); ++i) {
		_extraCursor->setPosition(i + 1);
		emit symbolFormatChanged(_user->getUserId(), i, _extraCursor->charFormat());
	}
}

void TextEdit::fontChanged(const QFont& f)
{
	//Se cambia il carattere aggiorno i ComboBox con dimensione e famiglia
	comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));

	//Setto i rispettivi pulsanti checkati
	actionTextBold->setChecked(f.bold());
	actionTextItalic->setChecked(f.italic());
	actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor& c)
{
	//Cambio icona colore se cambia il colore
	QPixmap pix(rsrcPath + "/textcolor.png");
	QBitmap mask = pix.createMaskFromColor(Qt::transparent, Qt::MaskInColor);
	pix.fill(c);
	pix.setMask(mask);
	actionTextColor->setIcon(pix);
}

//Questa funzione viene chiamata ogni volta che vengono effettuate modifiche al testo.

void TextEdit::contentsChange(int position, int charsRemoved, int charsAdded) {


	//Gestione cancellazione carattere
	if (charsRemoved > 0) {
		for (int i = 0; i < charsRemoved; ++i) {
			emit charDeleted(position);
		}
	}

	if (charsAdded > 0) {
		QTextCursor cursor = textEdit->textCursor();
		QTextBlockFormat blockFmt;
		int i;

		for (i = position; i < position + charsAdded; ++i) {
			//Getting QTextBlockFormat from cursor
			blockFmt = cursor.blockFormat();

			//Ricavo il carattere inserito
			QChar ch = textEdit->document()->characterAt(i);

			//Setto il cursore alla posizione+1 perchè il formato (charFormat) viene verificato sul carattere
			//precedente al cursore.
			cursor.setPosition(i + 1);

			//Getting QTextCharFormat from cursor
			QTextCharFormat fmt = cursor.charFormat();

			if ((i != position + charsAdded - 1) || (i != textEdit->document()->characterCount() - 1) || ch != QChar::ParagraphSeparator) {
				emit charInserted(ch, fmt, i);
			}
			if (ch == QChar::ParagraphSeparator) {
				emit blockFormatChanged(i, i, blockFmt);
			}
			if (charsAdded > 1) {
				emit blockFormatChanged(i, i, blockFmt);
			}
		}
	}
}

/*

Function to handle extra cursors position updates.
Recomputes all positions based on document scroll positions.

*/

void TextEdit::userCursorPositionChanged(qint32 position, qint32 user)
{
	//To change with unique id
	Presence* p = onlineUsers.find(user).value();
	QTextCursor* cursor = p->cursor();
	QLabel* userCursorLabel = p->label();

	//Hide label to move it
	userCursorLabel->close();

	//Change usercursor position
	cursor->setPosition(position);

	//Draw cursor
	const QRect qRect = textEdit->cursorRect(*cursor);

	QPixmap pix(qRect.width() * 2.5, qRect.height());
	pix.fill(p->color());
	userCursorLabel->setPixmap(pix);

	//Show moved label
	userCursorLabel->move(qRect.left(), qRect.top());
	userCursorLabel->show();

}


void TextEdit::handleUsersCursors() {

	QMap<qint32, Presence*>::iterator it;

	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		if (it.key() != _user->getUserId()) {

			Presence* p = it.value();

			QTextCursor* cursor = p->cursor();
			QLabel* cursorLabel = p->label();

			cursorLabel->close();

			const QRect qRect = textEdit->cursorRect(*cursor);

			QPixmap pix(qRect.width() * 2.5, qRect.height());
			pix.fill(p->color());
			cursorLabel->setPixmap(pix);

			cursorLabel->move(qRect.left(), qRect.top());
			cursorLabel->show();
		}
	}
}


//Handle different users selections
void TextEdit::highlightUsersText()
{
	if (actionHighlightUsers->isChecked()) {
		QMap<qint32, Presence*>::iterator it;

		for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
			Presence* p = it.value();

			p->action()->setChecked(true);
		}

		handleMultipleSelections();
	}
	else {
		QList<QTextEdit::ExtraSelection> emptySelection;
		textEdit->setExtraSelections(emptySelection);

		QMap<qint32, Presence*>::iterator it;

		for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
			Presence* p = it.value();

			p->action()->setChecked(false);
		}
	}
}

void TextEdit::setExtraSelections(qint32 userId, QPair<int, int> selection)
{
	if (onlineUsers.contains(userId)) {
		Presence* p = onlineUsers.find(userId).value();
		QTextCursor* cursor = p->cursor();

		cursor->setPosition(selection.first);
		cursor->setPosition(selection.second, QTextCursor::KeepAnchor);

		QTextEdit::ExtraSelection userText;

		QColor color = p->color();
		color.setAlpha(70);
		userText.format.setBackground(color);
		userText.cursor = *cursor;

		p->addUserText(userText);
	}
}

void TextEdit::handleMultipleSelections()
{
	QList<QTextEdit::ExtraSelection> usersSelections;
	textEdit->setExtraSelections(usersSelections);

	QMap<qint32, Presence*>::iterator it;

	int actionsChecked = 0;

	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		Presence* p = it.value();

		if (p->action()->isChecked()) {
			usersSelections.append(p->userText());
			actionsChecked++;
		}
	}

	if (actionsChecked == 0) {
		actionHighlightUsers->setChecked(false);
	}

	if (actionsChecked == onlineUsers.size()) {
		actionHighlightUsers->setChecked(true);
	}


	textEdit->setExtraSelections(usersSelections);
}


void TextEdit::updateUsersSelections()
{
	QMap<qint32, Presence*>::iterator it;
	for (it = onlineUsers.begin(); it != onlineUsers.end(); it++) {
		it.value()->clearSelections();
	}

	emit generateExtraSelection();

	handleMultipleSelections();
}

void TextEdit::timerEvent(QTimerEvent* event)
{
	//if (textEdit->textCursor().position() != _currentCursorPosition) {
	_currentCursorPosition = textEdit->textCursor().position();
	emit newCursorPosition(textEdit->textCursor().position());
	//}

}
