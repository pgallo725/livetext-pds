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
	
	QPixmap pix(":/images/logo.png");
		int w = ui->label_logo->width();
		int h = ui->label_logo->height();
		ui->label_logo->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

	connect(ui->pushButton_login, SIGNAL(clicked()), this, SLOT(pushButtonLoginClicked()));
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
		//ui->stackedWidget->setCurrentIndex(2);
		//ui->stackedWidget->show();
		this->close();
		openEditor();
	}
	else {
		QMessageBox::warning(this, "Login", "Username and password is not correct");
	}
}


void WelcomeWindow::openEditor() {

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

