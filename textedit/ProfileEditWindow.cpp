#include "ProfileEditWindow.h"
#include "ui_profileeditwindow.h"

#include <QPixmap>
//#include <QDesktopWidget>
#include <QScreen>
#include <QFileDialog>
#include <QRegularExpressionValidator>

const QString rsrcPath = ":/images";


ProfileEditWindow::ProfileEditWindow(User& user, bool fromEditor, QWidget* parent)
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ProfileEditWindow),
	mngr(WidgetsManager(this)), _user(user), _editorFlag(fromEditor), _iconChanged(false)
{
	//UI setup
	ui->setupUi(this);

	//Center and resize
	// if (QApplication::desktop()->availableGeometry().size().width() <= 1366)
		if(QGuiApplication::primaryScreen()->availableGeometry().size().width()<=1366)
		mngr.centerAndResize(0.55, 0.62);
	else
		mngr.centerAndResize(0.5, 0.55);


	//Connects of ui elements
	connect(ui->pushButton_updateProfile, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->pushButton_browse, &QPushButton::clicked, this, &ProfileEditWindow::pushButtonBrowseClicked);
	connect(ui->pushButton_cancel, &QPushButton::clicked, this, &QDialog::close);

	connect(ui->lineEdit_editNick, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->lineEdit_editPsw, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);
	connect(ui->lineEdit_editPsw, &QLineEdit::textChanged, this, &ProfileEditWindow::passwordEdited);
	connect(ui->lineEdit_editPswConf, &QLineEdit::returnPressed, this, &ProfileEditWindow::pushButtonUpdateClicked);

	//Radio button
	connect(ui->radioButton_customAvatar, &QRadioButton::toggled, this, &ProfileEditWindow::radioButtonPressed);
	connect(ui->radioButton_defaultAvatar, &QRadioButton::toggled, this, &ProfileEditWindow::radioButtonPressed);

	//Validator for nickname (no spaces)
	ui->lineEdit_editNick->setValidator(new QRegularExpressionValidator(QRegularExpression("^[^\\s]+$"), this));


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
	//If update was successful it closes the window
	mngr.hideLoadingScreen(loading);

	this->close();
}

void ProfileEditWindow::updateFailed(QString error)
{
	//If update went wrong it displays an error message
	mngr.hideLoadingScreen(loading);
	ui->label_incorrect_edit->setText(error);
}


/* ---------------- PUSH BUTTONS ----------------*/

void ProfileEditWindow::pushButtonBrowseClicked()
{
	//File dialog
	QFileDialog* fileSelector = new QFileDialog(this, "Choose your profile icon", QDir::homePath(), "Image files(*.png *.jpg *.bmp *.jpeg)");
	fileSelector->setFileMode(QFileDialog::ExistingFile);
	fileSelector->setAttribute(Qt::WA_DeleteOnClose, true);

	//Open file dialog, to choose profile picture
	fileSelector->open(this, SLOT(updateSelectedAvatar(const QString&)));
}

void ProfileEditWindow::pushButtonUpdateClicked()
{
	//Gets information from fields
	QString nick = ui->lineEdit_editNick->text();
	QString newPassword = ui->lineEdit_editPsw->text();
	QString newPasswordConf = ui->lineEdit_editPswConf->text();
	//If the image was not changed, an empty one is sent
	QImage userIcon = _iconChanged ? ui->label_UsrIcon->pixmap().toImage() : QImage();		


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
	emit accountUpdate(nick, userIcon, newPassword, _editorFlag);

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
		QPixmap default_pixmap{rsrcPath + "/misc/defaultProfile.png"};
		ui->label_UsrIcon->setPixmap(default_pixmap.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		_iconChanged = true;
	}
}


/* ---------------- GUI UPDATE ----------------*/

void ProfileEditWindow::updateSelectedAvatar(const QString& path)
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
				_iconChanged = true;
				return;
			}
		}
	}

	//Load default profile picture
	QPixmap default_pixmap(rsrcPath + "/misc/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default_pixmap.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


//Resets all fields
void ProfileEditWindow::resetFields()
{
	ui->label_incorrect_edit->setText("");
	ui->lineEdit_editNick->setText("");
	ui->lineEdit_editPsw->setText("");
	ui->lineEdit_editPswConf->setText("");

	//Load default profile picture
	QPixmap default_pixmap(rsrcPath + "/misc/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default_pixmap.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

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
	ui->label_userId->setText(QString::number(_user.getUserId()).rightJustified(4, '0'));
}
