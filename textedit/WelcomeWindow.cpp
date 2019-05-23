#include "WelcomeWindow.h"
#include "ui_welcomewindow.h"
#include "textedit.h"

#include <QMessageBox>
#include <QPixmap>
#include <QWidget>


#include <QApplication>
#include <QDesktopWidget>


const QString rsrcPath = ":/images/win";

WelcomeWindow::WelcomeWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::WelcomeWindow)  {
	
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));
	
	ui->setupUi(this);
	
	int w = ui->pushButton_new->width();
	int h = ui->pushButton_new->height();
	ui->pushButton_new->setIconSize(QSize::QSize(w, h));
	ui->pushButton_new->setIcon(QIcon::QIcon(rsrcPath + "/WelcomeWindow/new.png"));

	QPixmap logoPix(":/images/logo.png");
	w = ui->label_logo->width();
	h = ui->label_logo->height();
	ui->label_logo->setPixmap(logoPix.scaled(w, h, Qt::KeepAspectRatio));
	

	connect(ui->pushButton_login, SIGNAL(clicked()), this, SLOT(pushButtonLoginClicked()));
	connect(ui->pushButton_new, SIGNAL(clicked()), this, SLOT(openEditor()));

	ui->stackedWidget->setCurrentIndex(0);
}

WelcomeWindow::~WelcomeWindow()
{
	delete ui;
}



void WelcomeWindow::pushButtonLoginClicked()
{
	QString username = ui->lineEdit_usr->text();
	QString password = ui->lineEdit_psw->text();

	if (username == "test" && password == "test") {
		//QMessageBox::information(this, "Login", "Username and password is correct");
		//hide();
		ui->stackedWidget->setCurrentIndex(2);
		ui->stackedWidget->show();
		
	}
	else {
		QMessageBox::warning(this, "Login", "Username and password is not correct");
	}
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
	//mw.fileNew();


	//Mostra la finestra di mw formata
	mw->show();
}

