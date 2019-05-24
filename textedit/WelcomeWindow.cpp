#include "WelcomeWindow.h"
#include "ui_welcomewindow.h"
#include "textedit.h"

#include <QMessageBox>
#include <QPixmap>
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>

#include <QApplication>
#include <QDesktopWidget>



const QString rsrcPath = ":/images/win";

WelcomeWindow::WelcomeWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::WelcomeWindow) {
	//Cistruttore landing page
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);

	//Icona "New file"
	int w = ui->pushButton_new->width();
	int h = ui->pushButton_new->height();
	ui->pushButton_new->setIconSize(QSize::QSize(w, h));
	ui->pushButton_new->setIcon(QIcon::QIcon(rsrcPath + "/WelcomeWindow/new.png"));

	//Icona "Open from URI"
	ui->pushButton_openuri->setIconSize(QSize::QSize(w, h));
	ui->pushButton_openuri->setIcon(QIcon::QIcon(rsrcPath + "/WelcomeWindow/newuri.png"));

	//Logo applicazione
	QPixmap logoPix(":/images/logo.png");
	w = ui->label_logo->width();
	h = ui->label_logo->height();
	ui->label_logo->setPixmap(logoPix.scaled(w, h, Qt::KeepAspectRatio));


	//Connect dei segnali dei vari pushbutton
	connect(ui->pushButton_login, &QPushButton::clicked, this, &WelcomeWindow::pushButtonLoginClicked);
	connect(ui->pushButton_new, &QPushButton::clicked, this, &WelcomeWindow::pushButtonNewClicked);
	connect(ui->pushButton_register, &QPushButton::clicked, this, &WelcomeWindow::pushButtonRegisterClicked);
	connect(ui->pushButton_browse, &QPushButton::clicked, this, &WelcomeWindow::pushButtonBrowseClicked);
	connect(ui->pushButton_regConf, &QPushButton::clicked, this, &WelcomeWindow::pushButtonConfirmRegistrationClicked);

	//User Icon
	QPixmap userPix(rsrcPath + "/WelcomeWindow/defaultProfile.png");
	w = ui->label_UsrIcon->width();
	h = ui->label_UsrIcon->height();
	ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::KeepAspectRatio));
	connect(ui->lineEdit_UsrIconPath, &QLineEdit::textChanged, this, &WelcomeWindow::showUserIcon);

	//Setta indice a 0 (finestra di login) per lo Stacked Widget
	ui->stackedWidget->setCurrentIndex(0);
}

WelcomeWindow::~WelcomeWindow()
{
	delete ui;
}



void WelcomeWindow::pushButtonLoginClicked()
{
	//Prende i dati dalle caselle Login e Password
	QString username = ui->lineEdit_usr->text();
	QString password = ui->lineEdit_psw->text();

	//Controllo user e password
	if (username == "test" && password == "test") {
		//Apre seconda landing page con operazioni sui file
		ui->stackedWidget->setCurrentIndex(2);
		ui->stackedWidget->show();
	}
	else {
		QMessageBox::warning(this, "Login", "Username and password is not correct");
	}
}

//Apre la pagina dedicata alla registrazione
void WelcomeWindow::pushButtonRegisterClicked()
{
	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget->show();
}

//Apre il dialogo per la scelta dell'icona utente
void WelcomeWindow::pushButtonBrowseClicked()
{
	QString filename = QFileDialog::getOpenFileName(this, "Choose your profile icon",
		QDir::homePath(), "Image files(*.png *.jpg *.bmp) ;; All files (*.*)");

	//Setta nel form il path scelto
	ui->lineEdit_UsrIconPath->setText(filename);
}

//Conferma la registrazione
void WelcomeWindow::pushButtonConfirmRegistrationClicked()
{
	QString name = ui->lineEdit_regName->text();
	QString surname = ui->lineEdit_regSurname->text();
	QString username = ui->lineEdit_regUsr->text();
	QString password = ui->lineEdit_regPsw->text();
	QString passwordConf = ui->lineEdit_regPswConf->text();
	QString iconPath = ui->lineEdit_UsrIconPath->text();

	//Controllo se i dati sono stati inseriti correttamente
	if (name.isEmpty() || surname.isEmpty() || username.isEmpty() || password.isEmpty() || password.isEmpty()) {
		QMessageBox::warning(this, "Registration", "Please fill all the required fields");
		return;
	}
	//Controllo sulla corrispondenza password
	if (password != passwordConf) {
		QMessageBox::warning(this, "Registration", "Passwords does not match");
		return;
	}
	//Se non è stata settata un'icona si salva quella di default, altrimenti si usa quella inserita
	if (!iconPath.isEmpty() && fileExist(iconPath)) {
		QPixmap userPix(iconPath);
	}
	else {
		QPixmap userPix(rsrcPath + "/WelcomeWindow/defaultProfile.png");
	}
}

//Funzione per verificare se un file esiste e se non è una directory
bool WelcomeWindow::fileExist(QString path) {

	QFileInfo file(path);
	
	if (file.exists() && file.isFile()) {
		return true;
	}
	else {
		return false;
	}
}

//Slot attivato quando viene modificato il percorso dell'icona
//Se la nuova immagine esiste la visualizza altrimenti visualizza l'icona di default
void WelcomeWindow::showUserIcon(QString path)
{
	if (fileExist(path)) {
		QPixmap userPix(path);
		int w = ui->label_UsrIcon->width();
		int h = ui->label_UsrIcon->height();
		ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::IgnoreAspectRatio));
	}
	else {
		QPixmap userPix(rsrcPath + "/WelcomeWindow/defaultProfile.png");
		int w = ui->label_UsrIcon->width();
		int h = ui->label_UsrIcon->height();
		ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::KeepAspectRatio));
	}
}

//Quando viene aperto un nuovo file apre l'editor
void WelcomeWindow::pushButtonNewClicked()
{
	openEditor();
}

void WelcomeWindow::openEditor() {

	this->close();


	/*QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(a);*/

	//Crea l'oggetto TextEdit un wrapper di QTextEdit modificato per realizzare le funzioni base
	TextEdit* mw = new TextEdit();

	//Dimensione finestra
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(mw);

	//Applica la dimensione al TextEdit e lo mette nella finestra corretta
	mw->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
	mw->move((availableGeometry.width() - mw->width()) / 2, (availableGeometry.height() - mw->height()) / 2);

	//Apre il file example.html se lo trova, altrimenti ne crea uno nuovo come se lo passassi da linea di comando
	//if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))
	mw->fileNew();


	//Mostra la finestra di mw formata
	mw->show();
}

