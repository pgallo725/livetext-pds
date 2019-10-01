#include "LandingPage.h"
#include "ui_landingpage.h"
#include "textedit.h"


#include <QMessageBox>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMovie>
#include <QSplashScreen>

#include <QApplication>
#include <QDesktopWidget>


const QString rsrcPath = ":/images/win";

LandingPage::LandingPage(QWidget* parent) : QMainWindow(parent), ui(new Ui::LandingPage) {
	//Costruttore landing page
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
	//Setup dimensione finestra
	centerAndResize();


	//Crea l'oggetto NewFileWindow
	newFileWindow = new NewFileWindow(this);

	//Crea l'oggetto OpenUri
	openURIWindow = new OpenUriWindow(this);

	//Icona "New file"
	int w = ui->pushButton_new->width();
	int h = ui->pushButton_new->height();
	ui->pushButton_new->setIconSize(QSize::QSize(w, h));
	ui->pushButton_new->setIcon(QIcon::QIcon(rsrcPath + "/LandingPage/new.png"));

	//Icone Tab Widget
	ui->tabWidget->setTabIcon(0, QIcon::QIcon(rsrcPath + "/LandingPage/login.png"));
	ui->tabWidget->setTabIcon(1, QIcon::QIcon(rsrcPath + "/LandingPage/register.png"));
	ui->tabWidget->setIconSize(QSize(40, 65));


	//Icona Browse...
	ui->pushButton_browse->setIcon(QIcon(rsrcPath + "/fileopen.png"));


	//Icona "Open from URI"
	ui->pushButton_openuri->setIconSize(QSize::QSize(w, h));
	ui->pushButton_openuri->setIcon(QIcon::QIcon(rsrcPath + "/LandingPage/openuri.png"));

	//Icona back
	ui->pushButton_back->setIcon(QIcon::QIcon(rsrcPath + "/logout.png"));

	//Icona open
	ui->pushButton_open->setIcon(QIcon::QIcon(rsrcPath + "/filenew.png"));

	//Icona remove
	ui->pushButton_remove->setIcon(QIcon::QIcon(rsrcPath + "/LandingPage/remove.png"));

	//Logo applicazione
	QPixmap logoPix(":/images/logo.png");

	w = ui->label_logo->width();
	h = ui->label_logo->height();
	ui->label_logo->setPixmap(logoPix.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	//Tab widget cambiata
	connect(ui->tabWidget, &QTabWidget::currentChanged, this, &LandingPage::currentTabChanged);

	//Connect dei segnali dei vari pushbutton
	connect(ui->pushButton_confirmOperation, &QPushButton::clicked, this, &LandingPage::confirmOperation);
	connect(ui->pushButton_new, &QPushButton::clicked, this, &LandingPage::pushButtonNewClicked);
	connect(ui->pushButton_register, &QPushButton::clicked, this, &LandingPage::pushButtonRegisterClicked);
	connect(ui->pushButton_browse, &QPushButton::clicked, this, &LandingPage::pushButtonBrowseClicked);
	connect(ui->pushButton_open, &QPushButton::clicked, this, &LandingPage::pushButtonOpenClicked);
	connect(ui->pushButton_remove, &QPushButton::clicked, this, &LandingPage::pushButtonRemoveClicked);
	connect(ui->pushButton_openuri, &QPushButton::clicked, this, &LandingPage::pushButtonOpenUriClicked);
	connect(ui->pushButton_back, &QPushButton::clicked, this, &LandingPage::pushButtonBackClicked);

	//Connect tra le lineEdit di user/password e tasto invio per premere bottone di login
	connect(ui->lineEdit_psw, &QLineEdit::returnPressed, this, &LandingPage::confirmOperation);
	connect(ui->lineEdit_usr, &QLineEdit::returnPressed, this, &LandingPage::confirmOperation);

	//Connect lista file (QListWidget)
	connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &LandingPage::pushButtonOpenClicked);
	connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, &LandingPage::enablePushButtonOpen);

	//User Icon
	QPixmap userPix(rsrcPath + "/LandingPage/defaultProfile.png");
	w = ui->label_UsrIcon->width();
	h = ui->label_UsrIcon->height();
	ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	//Connect per lineEdit userIcon permette di aggiornare l'anteprima
	connect(ui->lineEdit_UsrIconPath, &QLineEdit::textChanged, this, &LandingPage::showUserIcon);

	//Setta indice a 0 (finestra di login) per lo Stacked Widget
	ui->stackedWidget->setCurrentIndex(0);

	//Indice a 0 (Login) del TabWidget
	ui->tabWidget->setCurrentIndex(0);
	ui->pushButton_confirmOperation->setText(tr("Login"));

	//Validator per non inserire lettere nei campi server/port
	ui->lineEdit_serverPort->setValidator(new QIntValidator(0, 10000, this));



	//If there's a saved credential it opens it
	QFile file("userLogin.dat");
	if (file.open(QIODevice::ReadOnly)) {
		QTextStream stream(&file);
		QString line = stream.readLine();

		if (!line.isEmpty()) {
			ui->checkBox_saveCredential->setChecked(true);
		}

		ui->lineEdit_usr->setText(line);

		line = stream.readLine();
		ui->lineEdit_serverIP->setText(line);

		line = stream.readLine();
		ui->lineEdit_serverPort->setText(line);
	}

	file.close();



	/* LOADING GIF */
	loading = new QLabel(this);
}

LandingPage::~LandingPage()
{
	delete ui;
}

void LandingPage::confirmOperation()
{
	QString serverIP = ui->lineEdit_serverIP->text();
	QString serverPort = ui->lineEdit_serverPort->text();


	//Controllo se i dati sono stati inseriti correttamente
	if (serverIP.isEmpty() || serverPort.isEmpty()) {
		ui->label_incorrect_operation->setText(tr("Please fill all the required fields"));
		return;
	}

	if (ui->tabWidget->currentIndex() == 0) {
		if (ui->lineEdit_usr->text().isEmpty() || ui->lineEdit_psw->text().isEmpty()) {
			ui->label_incorrect_operation->setText(tr("Please fill all the required fields"));
			return;
		}
	}
	else {
		if (ui->lineEdit_regUsr->text().isEmpty() || ui->lineEdit_regPsw->text().isEmpty() || ui->lineEdit_regPswConf->text().isEmpty()) {
			ui->label_incorrect_operation->setText(tr("Please fill all the required fields"));
			return;
		}

		//Controllo sulla corrispondenza password
		if (ui->lineEdit_regPsw->text() != ui->lineEdit_regPswConf->text()) {
			ui->label_incorrect_operation->setText(tr("Passwords does not match"));
			return;
		}
	}


	//Function to show loading animation
	startLoadingAnimation(tr("Connecting to server..."));

	QCoreApplication::processEvents();

	emit(connectToServer(serverIP, serverPort.toShort()));
}

void LandingPage::Login()
{
	/*
	//Bypass login
	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget->show();
	stopLoadingAnimation();
	return;
	*/

	//Prende i dati dalle caselle Login e Password
	QString username = ui->lineEdit_usr->text();
	QString password = ui->lineEdit_psw->text();


	QFile file("userLogin.dat");
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file);
		if (ui->checkBox_saveCredential->isChecked()) {
			stream << username << endl;
			stream << ui->lineEdit_serverIP->text() << endl;
			stream << ui->lineEdit_serverPort->text() << endl;
		}
		else {
			stream << "" << endl;
		}
	}

	file.close();

	emit(serverLogin(username, password));
}

//Conferma la registrazione
void LandingPage::Register()
{
	QString nickname = ui->lineEdit_regNick->text();
	QString username = ui->lineEdit_regUsr->text();
	QString password = ui->lineEdit_regPsw->text();
	QString passwordConf = ui->lineEdit_regPswConf->text();
	QImage userIcon = ui->label_UsrIcon->pixmap()->toImage();

	emit serverRegister(username, password, nickname, userIcon);
}


void LandingPage::currentTabChanged(int index)
{
	switch (index) {
	case 0:
		ui->lineEdit_regNick->setText("");
		ui->lineEdit_regUsr->setText("");
		ui->lineEdit_regPsw->setText("");
		ui->lineEdit_regPswConf->setText("");
		ui->lineEdit_UsrIconPath->setText("");
		ui->pushButton_confirmOperation->setText(tr("Login"));
		break;
	case 1:
		ui->lineEdit_psw->setText("");
		ui->lineEdit_usr->setText("");
		ui->pushButton_confirmOperation->setText(tr("Register"));
		break;
	}

}



void LandingPage::impossibleToConnect()
{
	stopLoadingAnimation();
	ui->label_incorrect_operation->setText(tr("Invalid server/port"));
}

void LandingPage::incorrectFileOperation(QString error)
{
	stopLoadingAnimation();
	if (openURIWindow->isVisible()) {
		openURIWindow->incorrectOperation(error);
	}
	else if (newFileWindow->isVisible()) {
		newFileWindow->incorrectOperation(error);
	}
	else {
		ui->label_incorrect_file_operation->setText(error);
	}

}

void LandingPage::connectionEstabilished()
{
	stopLoadingAnimation();
	switch (ui->tabWidget->currentIndex()) {
	case 0:
		Login();
		break;
	case 1:
		Register();
		break;
	}
}


void LandingPage::openLoggedPage()
{
	stopLoadingAnimation();

	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget->show();
}

void LandingPage::incorrectOperation(QString msg)
{
	stopLoadingAnimation();
	ui->label_incorrect_operation->setText(msg);
}

void LandingPage::documentDismissed()
{

	stopLoadingAnimation();
}

void LandingPage::pushButtonRegisterClicked()
{
	//Cancellazione campi login
	ui->label_incorrect_operation->setText("");
	ui->lineEdit_psw->setText("");
	ui->lineEdit_usr->setText("");

	//Switch alla pagina di registrazione
	ui->tabWidget->setCurrentIndex(1);
}


void LandingPage::pushButtonBrowseClicked()
{
	//Apre il dialogo per la scelta dell'icona utente
	QString filename = QFileDialog::getOpenFileName(this, "Choose your profile icon",
		QDir::homePath(), "Image files(*.png *.jpg *.bmp)");

	//Setta nel form il path scelto
	ui->lineEdit_UsrIconPath->setText(filename);
}

void LandingPage::pushButtonOpenClicked()
{
	QString fileSelected = ui->listWidget->currentItem()->text();
	if (fileSelected != "<No files found>") {
		startLoadingAnimation(tr("Opening document..."));
		QCoreApplication::processEvents();
		emit openDocument(ui->listWidget->currentRow());
	}

}

void LandingPage::pushButtonRemoveClicked()
{
	QString fileSelected = ui->listWidget->currentItem()->text();

	if (fileSelected != "<No files found>") {
		startLoadingAnimation(tr("Removing document..."));
		QCoreApplication::processEvents();
		emit removeDocument(ui->listWidget->currentRow());
	}
}

void LandingPage::pushButtonOpenUriClicked()
{
	//Mostra la finestra di mw formata
	openURIWindow->exec();
}

void LandingPage::pushButtonNewClicked()
{
	//Mostra la finestra di mw formata
	newFileWindow->exec();
}


void LandingPage::pushButtonBackClicked()
{
	ui->lineEdit_psw->setText("");
	ui->lineEdit_regUsr->setText("");
	ui->lineEdit_regNick->setText("");
	ui->lineEdit_regPsw->setText("");
	ui->lineEdit_regPswConf->setText("");
	ui->lineEdit_UsrIconPath->setText("");
	ui->label_incorrect_operation->setText("");

	ui->stackedWidget->setCurrentIndex(0);
	ui->stackedWidget->show();

	emit(serverLogout());
}

void LandingPage::enablePushButtonOpen()
{
	if (ui->listWidget->item(0)->text() != "<No files found>")
	{
		ui->pushButton_open->setEnabled(true);
		ui->pushButton_remove->setEnabled(true);
	}
	else {
		ui->pushButton_open->setEnabled(false);
		ui->pushButton_remove->setEnabled(false);
	}

}



void LandingPage::setupFileList(QList<URI> documents)
{
	ui->listWidget->clear();

	QList<URI>::iterator it;

	for (it = documents.begin(); it != documents.end(); it++) {
		ui->listWidget->addItem(new QListWidgetItem(QIcon(rsrcPath + "/LandingPage/richtext.png"), it->getDocumentName() + " (" + it->getAuthorName() + ")"));
	}


	//Se non vengono trovati files viene visualizzato "<No files found>"
	if (ui->listWidget->count() == 0) {
		ui->listWidget->addItem("<No files found>");
		ui->listWidget->item(0)->flags() & ~Qt::ItemIsSelectable;
	}

	ui->pushButton_remove->setEnabled(false);
	ui->pushButton_open->setEnabled(false);
}

void LandingPage::closeAll()
{
	openURIWindow->close();
	newFileWindow->close();
	this->close();
}


//Slot attivato quando viene modificato il percorso dell'icona
//Se la nuova immagine esiste la visualizza altrimenti visualizza l'icona di default
void LandingPage::showUserIcon(QString path)
{
	QFileInfo file(path);
	int w = ui->label_UsrIcon->width();
	int h = ui->label_UsrIcon->height();

	if (file.exists() && file.isFile()) {
		QPixmap userPix(path);

		if (!userPix.isNull()) {
			ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			ui->label_incorrect_operation->setText("");
			return;
		}

	}

	//Mostra errore in caso di immagine non visualizzabile
	ui->label_incorrect_operation->setText("Please choose a valid image file");

	QPixmap default(rsrcPath + "/LandingPage/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


void LandingPage::centerAndResize() {
	//Ricava dimensione desktop
	QSize availableSize = QApplication::desktop()->availableGeometry().size();
	int width = availableSize.width();
	int height = availableSize.height();

	//Proporzionamento
	width *= 0.7;
	height *= 0.8;

	//Le dimensioni vengono fissate per rendere la finestra non resizable
	setMaximumHeight(height);
	setMinimumHeight(height);
	setMaximumWidth(width);
	setMinimumWidth(width);

	//Nuova dimensione
	QSize newSize(width, height);

	//Crea il nuovo rettangolo su cui aprire la finestra
	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, newSize, QApplication::desktop()->availableGeometry()));
}

void LandingPage::startLoadingAnimation(QString text)
{

	//loading->setMovie(movie);
	loading->setText(text);
	QFont font = loading->font();
	font.setPointSize(25);

	loading->setAlignment(Qt::AlignCenter);

	loading->setFont(font);

	//Center and resize
	loading->resize(QSize(512, 128));

	loading->move(width() / 2 - 256, height() / 2 - 64);

	QApplication::setOverrideCursor(Qt::WaitCursor);

	loading->show();

	//Disable main window until client->Connect(...)return the result of the connection
	setEnabled(false);
}

void LandingPage::stopLoadingAnimation()
{
	QApplication::restoreOverrideCursor();
	loading->close();
	setEnabled(true);
}
