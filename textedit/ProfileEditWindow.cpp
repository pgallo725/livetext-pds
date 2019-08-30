#include "ProfileEditWindow.h"

#include "ui_profileeditwindow.h"
#include "textedit.h"

#include <QMessageBox>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>

#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

const QString rsrcPath = ":/images/win";

ProfileEditWindow::ProfileEditWindow(User* user, QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ProfileEditWindow) {
	//Costruttore landing page
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);

	//Setting di dimensione e posizione finestra
	centerAndResize();

	//Connect dei segnali dei vari pushbutton
	connect(ui->pushButton_updateProfile, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->pushButton_browse, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonBrowseClicked);
	connect(ui->pushButton_cancel, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonCancelClicked);

	//User icon
	//QPixmap userPix(rsrcPath + "/LandingPage/defaultProfile.png");
	QPixmap userPix;
	int w = ui->label_UsrIcon->width();
	int h = ui->label_UsrIcon->height();

	userPix.convertFromImage(user->getIcon());
	ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	//Connect per lineEdit userIcon permette di aggiornare l'anteprima
	connect(ui->lineEdit_UsrIconPath, &QLineEdit::textChanged, this, &ProfileEditWindow::showUserIcon);


	//Set username
	ui->label_username->setText(user->getUsername());
	ui->lineEdit_editNick->setText(user->getNickname());
	ui->lineEdit_editPswConf->setText(user->getPassword());
	ui->lineEdit_editPsw->setText(user->getPassword());
}

ProfileEditWindow::~ProfileEditWindow()
{
	delete ui;
}

void ProfileEditWindow::updateSuccessful()
{
	this->close();
}

void ProfileEditWindow::updateFailed(QString error)
{
	ui->label_incorrect_edit->setText(error);
}

void ProfileEditWindow::pushButtonBrowseClicked()
{
	//Apre il dialogo per la scelta dell'icona utente
	QString filename = QFileDialog::getOpenFileName(this, "Choose your profile icon",
		QDir::homePath(), "Image files(*.png *.jpg *.bmp) ;; All files (*.*)");

	//Setta nel form il path scelto
	ui->lineEdit_UsrIconPath->setText(filename);
}

void ProfileEditWindow::pushButtonCancelClicked()
{
	this->close();
}

void ProfileEditWindow::showUserIcon(QString path)
{
	QFileInfo file(path);
	int w = ui->label_UsrIcon->width();
	int h = ui->label_UsrIcon->height();

	//Controllo che immagine esista e sia un file
	if (file.exists() && file.isFile()) {
		QPixmap userPix(path);

		if (!userPix.isNull()) {
			ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			ui->label_incorrect_edit->setText("");
			return;
		}

	}

	//Mostra errore in caso di immagine non visualizzabile
	ui->label_incorrect_edit->setText("Please choose a valid image file");

	QPixmap default(rsrcPath + "/LandingPage/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


void ProfileEditWindow::pushButtonUpdateClicked()
{

	QString nick = ui->lineEdit_editNick->text();
	QString newPassword = ui->lineEdit_editPsw->text();
	QString newPasswordConf = ui->lineEdit_editPswConf->text();
	QImage userIcon = ui->label_UsrIcon->pixmap()->toImage();


	//Controllo sulla corrispondenza password
	if (!newPassword.isEmpty() || !newPasswordConf.isEmpty()) {
		if (newPassword != newPasswordConf) {
			ui->label_incorrect_edit->setText("Passwords does not match");
			return;
		}
	}


	emit(accountUpdate(nick, userIcon, newPassword));
}



void ProfileEditWindow::centerAndResize() {
	//Ricava dimensione desktop
	QSize availableSize = QApplication::desktop()->availableGeometry().size();
	int width = availableSize.width();
	int height = availableSize.height();

	//Proporzionamento
	width *= 0.4;
	height *= 0.5;

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