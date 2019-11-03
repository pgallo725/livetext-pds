#include "ProfileEditWindow.h"

#include "ui_profileeditwindow.h"
#include "textedit.h"

#include <QMessageBox>
#include <QPixmap>
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

const QString rsrcPath = ":/images";

ProfileEditWindow::ProfileEditWindow(User& user, QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ProfileEditWindow), _user(user), mngr(WidgetsManager(this)) {
	//UI setup
	ui->setupUi(this);

	//Center and resize
	if (QApplication::desktop()->availableGeometry().size().width() <= 1366)
		mngr.centerAndResize(0.6, 0.75);
	else
		mngr.centerAndResize(0.5, 0.55);


	//Connects of ui elements
	connect(ui->pushButton_updateProfile, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->pushButton_browse, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonBrowseClicked);
	connect(ui->pushButton_cancel, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonCancelClicked);

	connect(ui->lineEdit_editNick, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->lineEdit_editPsw, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->lineEdit_editPsw, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->lineEdit_editPswConf, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);

	//Update GUI according to user info
	updateInfo();

	//Setups splash screen to show loading informations
	loading = new QLabel(this);
	mngr.setupLoadingInfo(loading);
}

ProfileEditWindow::~ProfileEditWindow()
{
	delete ui;
}

/* ---------------- ERRORS/MESSAGES ----------------*/

void ProfileEditWindow::updateSuccessful()
{
	//If update went successful it closes the window
	mngr.hideLoadingScreen(loading);

	//Reset fields
	resetFields();

	//Closes window
	this->close();
}

void ProfileEditWindow::updateFailed(QString error)
{
	//if update went wrong it displays an error message
	mngr.hideLoadingScreen(loading);
	ui->label_incorrect_edit->setText(error);
}



/* ---------------- PUSH BUTTONS ----------------*/

void ProfileEditWindow::pushButtonBrowseClicked()
{
	//Open file dialog, to choose profile picture
	QString filename = QFileDialog::getOpenFileName(this, "Choose your profile icon",
		QDir::homePath(), "Image files(*.png *.jpg *.bmp)");

	//Sets in path box correct path
	ui->lineEdit_UsrIconPath->setText(filename);
}

void ProfileEditWindow::pushButtonCancelClicked()
{
	resetFields();
	this->close();
}


void ProfileEditWindow::pushButtonUpdateClicked()
{
	//Gets information from fields
	QString nick = ui->lineEdit_editNick->text();
	QString newPassword = ui->lineEdit_editPsw->text();
	QString newPasswordConf = ui->lineEdit_editPswConf->text();
	QImage userIcon = ui->label_UsrIcon->pixmap()->toImage();


	//Check if all password are the same (if setted)
	if (!newPassword.isEmpty() || !newPasswordConf.isEmpty()) {
		if (newPassword != newPasswordConf) {
			ui->label_incorrect_edit->setText("Passwords does not match");
			return;
		}
	}

	//Shows loading splash screen 
	mngr.showLoadingScreen(loading, tr("Updating profile..."));


	//Check if some fields are unchanged it emits empty ones
	if (nick == _user.getNickname())
		nick = QString();
	
	if (userIcon == _user.getIcon())
		userIcon = QImage();


	//Sends new info to server
	emit(accountUpdate(nick, userIcon, newPassword));
}

/* ---------------- GUI UPDATE ----------------*/

void ProfileEditWindow::showUserIcon(QString path)
{

	QFileInfo file(path);

	//Preview size
	int w = ui->label_UsrIcon->width();
	int h = ui->label_UsrIcon->height();

	//Checks if file is an existing and if it is vaild
	if (file.exists() && file.isFile()) {
		//Create a pixmap from choosen path
		QPixmap userPix(path);

		//If the path is a valid image file sets the preview and clears all error
		if (!userPix.isNull()) {
			ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			ui->label_incorrect_edit->setText("");
			return;
		}

	}

	//If the file is not an image or is corrupted it shows an error message
	ui->label_incorrect_edit->setText("Please choose a valid image file");

	//And resets default profile icon
	QPixmap default(rsrcPath + "/misc/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


//Resets all fields
void ProfileEditWindow::resetFields()
{
	ui->label_incorrect_edit->setText("");
	ui->lineEdit_editNick->setText("");
	ui->lineEdit_editPsw->setText("");
	ui->lineEdit_editPswConf->setText("");
	ui->lineEdit_UsrIconPath->setText("");
}

void ProfileEditWindow::updateInfo()
{
	resetFields();

	//User profile picture
	QPixmap userPix;
	userPix.convertFromImage(_user.getIcon());

	int w = ui->label_UsrIcon->width();
	int h = ui->label_UsrIcon->height();
	ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	//Any changes in user profile picture path triggers a functions that updates preview
	connect(ui->lineEdit_UsrIconPath, &QLineEdit::textChanged, this, &ProfileEditWindow::showUserIcon);


	//Set username
	ui->label_username->setText(_user.getUsername());
	ui->lineEdit_editNick->setText(_user.getNickname());
}
