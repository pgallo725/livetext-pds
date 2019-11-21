#include "ProfileEditWindow.h"

#include "ui_profileeditwindow.h"
#include "textedit.h"

#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QDesktopWidget>

const QString rsrcPath = ":/images";


ProfileEditWindow::ProfileEditWindow(User& user, QWidget* parent) 
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ProfileEditWindow),
	_user(user), _iconChanged(false), mngr(WidgetsManager(this)) 
{
	//UI setup
	ui->setupUi(this);

	//Center and resize
	if (QApplication::desktop()->availableGeometry().size().width() <= 1366)
		mngr.centerAndResize(0.55, 0.7);
	else
		mngr.centerAndResize(0.5, 0.55);


	//Connects of ui elements
	connect(ui->pushButton_updateProfile, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->pushButton_browse, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonBrowseClicked);
	connect(ui->pushButton_cancel, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonCancelClicked);

	connect(ui->lineEdit_editNick, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->lineEdit_editPsw, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->lineEdit_editPsw, &QLineEdit::textChanged, this, &ProfileEditWindow::passwordEdited);
	connect(ui->lineEdit_editPswConf, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);

	//Radio button
	connect(ui->radioButton_customAvatar, &QRadioButton::toggled, this, &ProfileEditWindow::radioButtonPressed);
	connect(ui->radioButton_defaultAvatar, &QRadioButton::toggled, this, &ProfileEditWindow::radioButtonPressed);

	//Update GUI according to user info
	updateInfo();

	//Browse icon
	ui->pushButton_browse->setIcon(QIcon(rsrcPath + "/landingPage/fileopen.png"));

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
		QDir::homePath(), "Image files(*.png *.jpg *.bmp *.jpeg)");

	//Sets in path box correct path
	if (!filename.isEmpty()) {
		updateUserAvatarPreview(filename);
		_iconChanged = true;
	}
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
	QImage userIcon = _iconChanged ? 
		ui->label_UsrIcon->pixmap()->toImage() : QImage();		//If the image was not changed, an empty one is sent


	//Check if all password are the same (if setted)
	if (!newPassword.isEmpty() || !newPasswordConf.isEmpty()) {
		if (newPassword != newPasswordConf) {
			ui->label_incorrect_edit->setText(tr("Passwords don't match"));
			return;
		}
	}

	//Shows loading splash screen 
	mngr.showLoadingScreen(loading, tr("Updating profile..."));

	//Sends new info to server
	emit accountUpdate(nick, userIcon, newPassword, _fromEditor);

	_iconChanged = false;	 // Reset the image flag
}

void ProfileEditWindow::passwordEdited()
{
	if (ui->lineEdit_editPsw->text().isEmpty())
	{
		ui->lineEdit_editPswConf->clear();
		ui->lineEdit_editPswConf->setEnabled(false);
	}
	else
	{
		ui->lineEdit_editPswConf->setEnabled(true);
	}
}

void ProfileEditWindow::radioButtonPressed()
{
	if (ui->radioButton_customAvatar->isChecked()) {
		ui->pushButton_browse->setEnabled(true);
	}
	else
	{
		ui->pushButton_browse->setEnabled(false);
		ui->label_incorrect_edit->setText("");
		ui->label_imageSize->setText("");

		//Load default profile picture
		QPixmap default(rsrcPath + "/misc/defaultProfile.png");
		ui->label_UsrIcon->setPixmap(default.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		_iconChanged = true;
	}
}


/* ---------------- GUI UPDATE ----------------*/

void ProfileEditWindow::updateUserAvatarPreview(QString path)
{
	QFileInfo file(path);

	//Check if file in path exist and if it is a valid image file
	if (file.exists() && file.isFile()) {
		QPixmap userPix(path);
		if (userPix.isNull()) {
			//Shows error
			ui->label_incorrect_edit->setText(tr("Please choose a valid image file"));
			ui->label_imageSize->setText("");
		}
		else
		{
			qint64 fileSize = file.size();
			if (fileSize > MAX_IMAGE_SIZE) {
				//Shows error
				ui->label_incorrect_edit->setText(tr("Selected image is too big, please choose another one (Maximum size: 1MB)"));
				return;
			}
			else {
				ui->label_imageSize->setText("Image size: " + QString::number(fileSize / 1024) + " KB");
				ui->label_UsrIcon->setPixmap(userPix.scaled(ui->label_UsrIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
				ui->label_incorrect_edit->setText("");
				return;
			}
		}
	}

	//Load default profile picture
	QPixmap default(rsrcPath + "/misc/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


//Resets all fields
void ProfileEditWindow::resetFields()
{
	ui->label_incorrect_edit->setText("");
	ui->lineEdit_editNick->setText("");
	ui->lineEdit_editPsw->setText("");
	ui->lineEdit_editPswConf->setText("");

	//Load default profile picture
	QPixmap default(rsrcPath + "/misc/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

	//Check radio-button
	ui->radioButton_customAvatar->setChecked(true);
}

void ProfileEditWindow::updateInfo()
{
	resetFields();

	//User profile picture
	QPixmap userPix;
	userPix.convertFromImage(_user.getIcon());

	ui->label_UsrIcon->setPixmap(userPix.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

	//Set username
	ui->label_username->setText(_user.getUsername());
	ui->lineEdit_editNick->setText(_user.getNickname());
}

void ProfileEditWindow::setFromEditor(bool fromEditor)
{
	_fromEditor = fromEditor;
}
