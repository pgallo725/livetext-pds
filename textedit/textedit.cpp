#include <QAction>
#include <QApplication>
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
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
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
#include <QKeyEvent>

const QString rsrcPath = ":/images/win";

TextEdit::TextEdit(QWidget* parent) : QMainWindow(parent)
{
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));

	textEdit = new QTextEdit(this);

	// Permettono di connettere le funzioni di QTextEdit con quelle di TextEdit ovvero:
	// quando viene invocata la funzione currentCharFormatChanged genera un segnale che viene recepito
	// dall' oggetto TextEdit (this) che invoca la funzione (slot) currentCharFormatChanged
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::currentCharFormatChanged);
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::cursorPositionChanged);

	//Assegna il Widget textEdit alla finestra principaòe
	setCentralWidget(textEdit);

	//Serve su GNOME per far cambiare lo stile dei buttons in relazione all'ambiente in cui sei
	//setToolButtonStyle(Qt::ToolButtonFollowStyle);

	//Funzioni definite sotto e chiamate nel costruttore
	setupFileActions();
	setupEditActions();
	setupTextActions();
	setupShareActions();
	setupUserActions();

	//Aggiunge un menu di Help alla barra dei menu con le conseguenti azioni (rimosso)
	/*
	{
		QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
		helpMenu->addAction(tr("About"), this, &TextEdit::about);
		helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
	}*/

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
	connect(textEdit->document(), &QTextDocument::modificationChanged, actionSave, &QAction::setEnabled);
	connect(textEdit->document(), &QTextDocument::modificationChanged, this, &QWidget::setWindowModified);
	connect(textEdit->document(), &QTextDocument::undoAvailable, actionUndo, &QAction::setEnabled);
	connect(textEdit->document(), &QTextDocument::redoAvailable, actionRedo, &QAction::setEnabled);

	connect(textEdit->document(), &QTextDocument::contentsChange, this, &TextEdit::contentsChange);


	//Stesso discorso delle connect ma inizializza
	setWindowModified(textEdit->document()->isModified());
	actionSave->setEnabled(textEdit->document()->isModified());
	actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
	actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

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
	
	//Stringa vuota
	setCurrentFileName(QString());
}

void TextEdit::closeEvent(QCloseEvent* e)
{
	if (maybeSave())
		e->accept();
	else
		e->ignore();
}

/*
	Questa funzione genera il menu di File Modifica ...
*/
void TextEdit::setupFileActions()
{
	//Creo una toolbar
	QToolBar* tb = addToolBar(tr("File Actions"));

	//Creo il menu File
	QMenu* menu = menuBar()->addMenu(tr("&File"));

	//Assegno icona per nuovo file
	const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));

	//Creo l'azione riferita alla creazione del nuovo file chiama il metodo fileNew
	QAction * a = menu->addAction(newIcon, tr("&New"), this, &TextEdit::fileNew);

	//Aggiungo action alla toolbar
	tb->addAction(a);

	//Assegno priorita all'action
	a->setPriority(QAction::LowPriority);

	//Aggiunge scorciatoia di tasti
	a->setShortcut(QKeySequence::New);


	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png"));
	a = menu->addAction(openIcon, tr("&Open..."), this, &TextEdit::fileOpen);
	a->setShortcut(QKeySequence::Open);
	tb->addAction(a);

	//Aggiunge separatore al menu
	menu->addSeparator();

	const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));
	actionSave = menu->addAction(saveIcon, tr("&Save"), this, &TextEdit::fileSave);
	actionSave->setShortcut(QKeySequence::Save);
	actionSave->setEnabled(false); //Se salvo disabilita il save
	tb->addAction(actionSave);

	a = menu->addAction(tr("Save &As..."), this, &TextEdit::fileSaveAs);
	a->setPriority(QAction::LowPriority);
	menu->addSeparator();

#ifndef QT_NO_PRINTER
	const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));
	a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
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
	QToolBar* tb = addToolBar(tr("Account"));
	QMenu* menu = menuBar()->addMenu(tr("&Account"));

	const QIcon userIcon = QIcon::fromTheme("user", QIcon(rsrcPath + "/user.png"));
	actionUser = menu->addAction(userIcon, tr("&Profile"), this, &TextEdit::editProfile);
	tb->addAction(actionUser);
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
	if (const QMimeData * md = QApplication::clipboard()->mimeData())
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
	QActionGroup * alignGroup = new QActionGroup(this);
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

	menu->addSeparator();

	//Gestione colore del testo
	//QPixMap crea l'icona colorata a seconda del colore che selezioniamo
	QPixmap pix(rsrcPath + "/textcolor.png");
	actionTextColor = menu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);
	tb->addAction(actionTextColor);

	tb = addToolBar(tr("Format Actions"));
	tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBarBreak(Qt::TopToolBarArea);
	addToolBar(tb);

	//Creo il combobox per lo stile del testo -- Da modificare a piacimento
	comboStyle = new QComboBox(tb);
	tb->addWidget(comboStyle);
	comboStyle->addItem("Standard");
	comboStyle->addItem("Bullet List (Disc)");
	comboStyle->addItem("Bullet List (Circle)");
	comboStyle->addItem("Bullet List (Square)");
	comboStyle->addItem("Ordered List (Decimal)");
	comboStyle->addItem("Ordered List (Alpha lower)");
	comboStyle->addItem("Ordered List (Alpha upper)");
	comboStyle->addItem("Ordered List (Roman lower)");
	comboStyle->addItem("Ordered List (Roman upper)");
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


//Funzione per apertura file
//Il file viene aperto in sola lettura e viene cambiato il nome del file corrente in quello appena aperto
//copiandoci il contenuto all'interno

bool TextEdit::load(const QString & f)
{
	//Se file non esiste torna false
	if (!QFile::exists(f))
		return false;

	//Apre il file in sola lettura
	QFile file(f);
	if (!file.open(QFile::ReadOnly))
		return false;

	//Legge tutto contenuto file
	QByteArray data = file.readAll();

	//Cerca di interpretare codice html
	QTextCodec* codec = Qt::codecForHtml(data);

	//Converte i dati i letti in Unicode
	QString str = codec->toUnicode(data);

	//Se è richText lo setta HTML altrimenti plain text
	if (Qt::mightBeRichText(str)) {
		textEdit->setHtml(str);
	}
	else {
		str = QString::fromLocal8Bit(data);
		textEdit->setPlainText(str);
	}

	setCurrentFileName(f);
	return true;
}

//Pop-up in caso di chiusura
bool TextEdit::maybeSave()
{

	//Solo se modificato altrimenti chiude
	if (!textEdit->document()->isModified())
		return true;

	//Acquisisce il valore del tasto premuto in in chiusura
	const QMessageBox::StandardButton ret =
		QMessageBox::warning(this, QCoreApplication::applicationName(),
			tr("The document has been modified.\nDo you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	//Se premo salva salva il documento
	if (ret == QMessageBox::Save)
		return fileSave();
	else if (ret == QMessageBox::Cancel)
		return false;
	return true;
}


void TextEdit::setCurrentFileName(const QString & fileName)
{
	this->fileName = fileName;
	textEdit->document()->setModified(false);

	//Assegna il titolo alla finestra, se vuoto untitled.txt
	QString shownName;
	if (fileName.isEmpty())
		shownName = "untitled.txt";
	else
		shownName = QFileInfo(fileName).fileName();

	//Sulla finestra appare nomeFile - nomeApplicazione
	setWindowTitle(tr("%1[*] - %2").arg(shownName, QCoreApplication::applicationName()));
	setWindowModified(false); //Il documento non ha modifiche non salvate
}

//Nuovo file, se ho modifiche non salvate chiede se salvare
void TextEdit::fileNew()
{
	if (maybeSave()) {
		//Cancella editor
		textEdit->clear();
		//Mette il nuovo file untitled.txt
		setCurrentFileName(QString());
	}
}

void TextEdit::fileOpen()
{
	//Apre finestra di selezione file con titolo "Open File..."
	QFileDialog fileDialog(this, tr("Open File..."));

	//Indica che se faccio ok apre
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	//Lista tutti i file e no permette di aprire tipo directory
	fileDialog.setFileMode(QFileDialog::ExistingFile);

	//Permette nel filtro di scegliere tra txt, html... 
	fileDialog.setMimeTypeFilters(QStringList() << "text/html" << "text/plain");

	// exec() permette di aprire la finestra di dialogo e non la chiude finchè l'utente non fa un'azione
	// Se chiudo la finestra non fa nulla
	if (fileDialog.exec() != QDialog::Accepted)
		return;

	//Sceglie il file selezionato (il primo)
	const QString fn = fileDialog.selectedFiles().first();

	//Chiama load per caricarlo con messaggio nel caso di successo/errore nella status bar in basso
	//toNativeSeparators scrive tutto il path fino al file name
	if (load(fn))
		statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fn)));
	else
		statusBar()->showMessage(tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(fn)));
}

bool TextEdit::fileSave()
{
	//Se il file è nuovo o inizia per :/ (untitled.txt) apre Save As...
	if (fileName.isEmpty())
		return fileSaveAs();
	if (fileName.startsWith(QStringLiteral(":/")))
		return fileSaveAs();

	//Crea interfaccia per scrivere il QTextDocument in un file passato come parametro
	QTextDocumentWriter writer(fileName);
	//Scrive il documento su file
	bool success = writer.write(textEdit->document());
	if (success) {
		//Se il salvataggio ha successo setta il documento come non modificato
		textEdit->document()->setModified(false);
		//Scrive sulla status bar "Wrote PATH"
		statusBar()->showMessage(tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName)));
	}
	else {
		//Altrimenti scrive sulla status bar showMessage
		statusBar()->showMessage(tr("Could not write to file \"%1\"").arg(QDir::toNativeSeparators(fileName)));
	}
	return success;
}

bool TextEdit::fileSaveAs()
{
	//Apre finestra salva con nome
	QFileDialog fileDialog(this, tr("Save as..."));
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);

	//Mette una lista di tipi disponibili per il salvataggio .odt .html .txt
	QStringList mimeTypes;
	mimeTypes << "application/vnd.oasis.opendocument.text" << "text/html" << "text/plain";
	fileDialog.setMimeTypeFilters(mimeTypes);

	//Default .odt
	fileDialog.setDefaultSuffix("odt");
	if (fileDialog.exec() != QDialog::Accepted)
		return false;

	//Prende o il file selezionato o quello scritto e sovrascrive nome file corrente, poi chiama FileSave
	const QString fn = fileDialog.selectedFiles().first();
	setCurrentFileName(fn);
	return fileSave();
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

void TextEdit::printPreview(QPrinter * printer)
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
	//TODO
}

void TextEdit::editProfile()
{
	//TODO
}

void TextEdit::textBold()
{
	QTextCharFormat fmt;
	//Controlla se l'azione TextBold è attivata e imposta il Weight normale/bold
	fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(actionTextUnderline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(actionTextItalic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

//Tipo di carattere (Arial...)
void TextEdit::textFamily(const QString & f)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(f);
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString & p)
{
	//Casta a float (?) e imposta la dimensione del carattere
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void TextEdit::textStyle(int styleIndex)
{
	//Prendo il cursore
	QTextCursor cursor = textEdit->textCursor();

	//Creo un oggetto stile carattere come undefined (Standard)
	QTextListFormat::Style style = QTextListFormat::ListStyleUndefined;

	//A seconda del combobox sovrascrivo stile
	switch (styleIndex) {
	case 1:
		style = QTextListFormat::ListDisc;
		break;
	case 2:
		style = QTextListFormat::ListCircle;
		break;
	case 3:
		style = QTextListFormat::ListSquare;
		break;
	case 4:
		style = QTextListFormat::ListDecimal;
		break;
	case 5:
		style = QTextListFormat::ListLowerAlpha;
		break;
	case 6:
		style = QTextListFormat::ListUpperAlpha;
		break;
	case 7:
		style = QTextListFormat::ListLowerRoman;
		break;
	case 8:
		style = QTextListFormat::ListUpperRoman;
		break;
	default:
		break;
	}

	//Indica l'inizio dell'editing a cui si appoggi l'undo/redo
	cursor.beginEditBlock();

	//Salva il formato del blocco
	QTextBlockFormat blockFmt = cursor.blockFormat();

	//Se Standard lo stile
	if (style == QTextListFormat::ListStyleUndefined) {

		blockFmt.setObjectIndex(-1); //(?)

		//Per evitare tutti gli heading mette che l'index del combobox 9 = H1 , 10 = H2...
		int headingLevel = styleIndex >= 9 ? styleIndex - 9 + 1 : 0; // H1 to H6, or Standard
		blockFmt.setHeadingLevel(headingLevel);
		cursor.setBlockFormat(blockFmt);

		int sizeAdjustment = headingLevel ? 4 - headingLevel : 0; // H1 to H6: +3 to -2

		//Crea formattazione carattere
		QTextCharFormat fmt;
		//Se ho degli heading mette grassetto
		fmt.setFontWeight(headingLevel ? QFont::Bold : QFont::Normal);
		//Aggiorna il size da +3 a -2 a seconda dell'Heading
		fmt.setProperty(QTextFormat::FontSizeAdjustment, sizeAdjustment);

		//Indica l'intera linea su cui sta il cursore
		cursor.select(QTextCursor::LineUnderCursor);
		cursor.mergeCharFormat(fmt);
		textEdit->mergeCurrentCharFormat(fmt);
	}
	else {

		QTextListFormat listFmt;
		//Indica se il cursore è gia in una lista, se sì ne prende il formato
		if (cursor.currentList()) {
			listFmt = cursor.currentList()->format();
		}
		else {
			//Altrimenti se non sono in una lista indento di +1
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);

			//Setto il formato del blocco
			cursor.setBlockFormat(blockFmt);
		}
		//Metto lo stile scelto nello switch come formato lista
		listFmt.setStyle(style);

		//Creo la lista indentata correttamente
		cursor.createList(listFmt);
	}

	cursor.endEditBlock();
}

void TextEdit::textColor()
{
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
void TextEdit::textAlign(QAction * a)
{

	//Applico gli allineamenti
	if (a == actionAlignLeft)
		textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	else if (a == actionAlignCenter)
		textEdit->setAlignment(Qt::AlignHCenter);
	else if (a == actionAlignRight)
		textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	else if (a == actionAlignJustify)
		textEdit->setAlignment(Qt::AlignJustify);
}

//Bind di QTextEdit quando cambia il formato
void TextEdit::currentCharFormatChanged(const QTextCharFormat & format)
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
	//Ne ricava lo stile della lista e lo setta nel combobox
	QTextList* list = textEdit->textCursor().currentList();
	if (list) {
		switch (list->format().style()) {
		case QTextListFormat::ListDisc:
			comboStyle->setCurrentIndex(1);
			break;
		case QTextListFormat::ListCircle:
			comboStyle->setCurrentIndex(2);
			break;
		case QTextListFormat::ListSquare:
			comboStyle->setCurrentIndex(3);
			break;
		case QTextListFormat::ListDecimal:
			comboStyle->setCurrentIndex(4);
			break;
		case QTextListFormat::ListLowerAlpha:
			comboStyle->setCurrentIndex(5);
			break;
		case QTextListFormat::ListUpperAlpha:
			comboStyle->setCurrentIndex(6);
			break;
		case QTextListFormat::ListLowerRoman:
			comboStyle->setCurrentIndex(7);
			break;
		case QTextListFormat::ListUpperRoman:
			comboStyle->setCurrentIndex(8);
			break;
		default:
			comboStyle->setCurrentIndex(-1);
			break;
		}
	}
	else {
		//Altrimenti setta nel combobox l'heading level corretto
		int headingLevel = textEdit->textCursor().blockFormat().headingLevel();
		comboStyle->setCurrentIndex(headingLevel ? headingLevel + 8 : 0);
	}
}

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
	//Se ho del testo negli appunti allora si sblocca il pulsante incolla
	if (const QMimeData * md = QApplication::clipboard()->mimeData())
		actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::about()
{
	//About del menu help (rimosso)
	QMessageBox::about(this, tr("About"), tr("This example demonstrates Qt's "
		"rich text editing facilities in action, providing an example "
		"document for you to experiment with."));
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat & format)
{
	//Chiamato quando devo cambiare il formato se ho una selezione
	QTextCursor cursor = textEdit->textCursor();
	if (!cursor.hasSelection())
		//Se non ho una selezione cambio alla parola in cui sta il cursore
		cursor.select(QTextCursor::WordUnderCursor);

	//Applico formattazione alla selezione
	cursor.mergeCharFormat(format);
	//Applico formattazione anche al documento
	textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::fontChanged(const QFont & f)
{
	//Se cambia il carattere aggiorno i ComboBox con dimensione e famiglia
	comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));

	//Setto i rispettivi pulsanti checkati
	actionTextBold->setChecked(f.bold());
	actionTextItalic->setChecked(f.italic());
	actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor & c)
{
	//Cambio icona colore se cambia il colore
	QPixmap pix(rsrcPath + "/textcolor.png");
	QBitmap mask = pix.createMaskFromColor(Qt::transparent, Qt::MaskInColor);
	pix.fill(c);
	pix.setMask(mask);
	actionTextColor->setIcon(pix);
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

//Questa funzione viene chiamata ogni volta che vengono effettuate modifiche al testo.

void TextEdit::contentsChange(int position, int charsRemoved, int charsAdded) {

	//Gestione cancellazione carattere
	if (charsRemoved > 0) {
		for (int i = position; i < position + charsRemoved; ++i) {
			//TODO: Remove characters
		}
	}

	if (charsAdded > 0) {
		//Gestione inserimento carattere
		QTextCursor cursor = textEdit->textCursor();

		for (int i = position; i < position + charsAdded; ++i) {
			//Setto il cursore alla posizione+1 perchè il formato (charFormat) viene verificato sul carattere
			//precedente al cursore.
			cursor.setPosition(i + 1);

			//Ricavo il carattere inserito
			QChar ch = textEdit->document()->characterAt(i);

			//Ricavo formato carattere inserio
			QTextCharFormat fmt = cursor.charFormat();

			//Controllo generico sulla proprietà del formato
			if (fmt.hasProperty(QTextFormat::FontItalic)) {
				return;
			}
		}
	}
}