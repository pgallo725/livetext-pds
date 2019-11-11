#include "LandingPage.h"
#include "ui_landingpage.h"

#include <QMessageBox>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainterPath>
#include <QFrame>
#include <QTableWidget>


#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "1500"

const QString rsrcPath = ":/images";

LandingPage::LandingPage(QWidget* parent) : QMainWindow(parent), ui(new Ui::LandingPage), mngr(WidgetsManager(this)) {
	//Window title
	setWindowTitle(QCoreApplication::applicationName());

	//Setup delle varie finestre ui
	ui->setupUi(this);

	//Center and resize
	if (QApplication::desktop()->availableGeometry().size().width() <= 1366)
		mngr.centerAndResize(0.7, 0.8);
	else
		mngr.centerAndResize(0.55, 0.65);


	//Creates new file window
	newFileWindow = new NewDocumentWindow(_buffer);

	//Create open from URI window
	openURIWindow = new OpenUriWindow(_buffer);

	//New file push button icon
	int w = ui->pushButton_new->width();
	int h = ui->pushButton_new->height();
	ui->pushButton_new->setIconSize(QSize::QSize(w, h));
	ui->pushButton_new->setIcon(QIcon(rsrcPath + "/landingPage/new.png"));

	//Tab widget icons
	ui->tabWidget->setTabIcon(0, QIcon(rsrcPath + "/landingPage/login.png"));
	ui->tabWidget->setTabIcon(1, QIcon(rsrcPath + "/landingPage/register.png"));
	ui->tabWidget->setIconSize(QSize(40, 65));

	//Browse icon
	ui->pushButton_browse->setIcon(QIcon(rsrcPath + "/landingPage/fileopen.png"));

	//Open from URI push button icon
	ui->pushButton_openuri->setIconSize(QSize::QSize(w, h));
	ui->pushButton_openuri->setIcon(QIcon(rsrcPath + "/landingPage/openuri.png"));

	//Logout
	ui->pushButton_back->setIcon(QIcon(rsrcPath + "/misc/logout.png"));

	//Open
	ui->pushButton_open->setIcon(QIcon(rsrcPath + "/landingPage/filenew.png"));

	//Remove
	ui->pushButton_remove->setIcon(QIcon(rsrcPath + "/landingPage/remove.png"));

	//LiveText main page logo
	QPixmap logoPix(rsrcPath + "/misc/logo.png");
	w = ui->label_logo->width();
	h = ui->label_logo->height();
	ui->label_logo->setPixmap(logoPix.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));


	/******************* CONNECTS *******************/
	/*
	*	Push buttons
	*	Line edit (return key pressed)
	*	File list
	*	Update user profile picture preview during registration
	*	tabWidget tab changed
	*/
	connect(ui->pushButton_confirmOperation, &QPushButton::clicked, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->pushButton_new, &QPushButton::clicked, this, &LandingPage::pushButtonNewClicked);
	connect(ui->pushButton_register, &QPushButton::clicked, this, &LandingPage::pushButtonRegisterClicked);
	connect(ui->pushButton_browse, &QPushButton::clicked, this, &LandingPage::pushButtonBrowseClicked);
	connect(ui->pushButton_open, &QPushButton::clicked, this, &LandingPage::pushButtonOpenClicked);
	connect(ui->pushButton_remove, &QPushButton::clicked, this, &LandingPage::pushButtonRemoveClicked);
	connect(ui->pushButton_openuri, &QPushButton::clicked, this, &LandingPage::pushButtonOpenUriClicked);
	connect(ui->pushButton_back, &QPushButton::clicked, this, &LandingPage::pushButtonBackClicked);
	connect(ui->pushButton_back, &QPushButton::clicked, this, &LandingPage::serverLogout);
	connect(ui->pushButton_editProfile, &QPushButton::clicked, this, &LandingPage::openEditProfile);

	connect(ui->lineEdit_psw, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->lineEdit_usr, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->lineEdit_serverIP, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->lineEdit_serverPort, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->lineEdit_regNick, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->lineEdit_regPsw, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->lineEdit_regPswConf, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);
	connect(ui->lineEdit_regUsr, &QLineEdit::returnPressed, this, &LandingPage::pushButtonConfirmOperationClicked);

	//Document list
	connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &LandingPage::enablePushButtonOpen);
	connect(ui->tableWidget, &QTableWidget::itemActivated, this, &LandingPage::pushButtonOpenClicked);


	//tabWidget
	connect(ui->tabWidget, &QTabWidget::currentChanged, this, &LandingPage::currentTabChanged);

	//Radio button
	connect(ui->radioButton_customAvatar, &QRadioButton::toggled, this, &LandingPage::radioButtonPressed);
	connect(ui->radioButton_defaultAvatar, &QRadioButton::toggled, this, &LandingPage::radioButtonPressed);


	//User Icon
	QPixmap userPix(rsrcPath + "/misc/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(userPix.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));


	//Qt/SSL logo
	QPixmap logos(rsrcPath + "/misc/logos.png");
	ui->label_logos->setPixmap(logos.scaled(ui->label_logos->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));



	//Index to 0 for stackedWidget (Home page)
	ui->stackedWidget->setCurrentIndex(0);


	//Index to 0 for tabWidget (Login)
	ui->tabWidget->setCurrentIndex(0);
	ui->pushButton_confirmOperation->setText(tr("Login"));	//Change puh button text according


	//Validator for server port
	ui->lineEdit_serverPort->setValidator(new QIntValidator(0, 65535, this));
	//Validator for server IP
	ui->lineEdit_serverIP->setValidator(new QRegExpValidator(QRegExp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])[\.]){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$"), this));


	//Loads user login infos
	loadUserLoginInfo();

	//If no info was found set default ones
	if (ui->lineEdit_serverIP->text().isEmpty()) {
		ui->lineEdit_serverIP->setText(DEFAULT_IP);
		ui->lineEdit_serverPort->setText(DEFAULT_PORT);
	}

	//Set proper field focus
	if (ui->lineEdit_usr->text().isEmpty())
		ui->lineEdit_usr->setFocus();
	else
		ui->lineEdit_psw->setFocus();


	//Setup loading message Label
	loading = new QLabel(this);
	mngr.setupLoadingInfo(loading);


}

LandingPage::~LandingPage()
{
	delete ui;
}

/******************* LOGIN/REGISTER *******************/
/*
*	Confirm operation (Connect to server)
*	Impossible to connect
*	Connection estabilished
*	Login/Register
*	Login successful
*/

void LandingPage::pushButtonConfirmOperationClicked()
{
	//Get server ip/port
	QString serverIP = ui->lineEdit_serverIP->text();
	QString serverPort = ui->lineEdit_serverPort->text();


	//Checks if all fields are filled properly
	if (serverIP.isEmpty() || serverPort.isEmpty()) {
		incorrectOperation(tr("Please fill all the required fields"));
		return;
	}

	if (ui->tabWidget->currentIndex() == 0) {
		if (ui->lineEdit_usr->text().isEmpty() || ui->lineEdit_psw->text().isEmpty()) {
			incorrectOperation(tr("Please fill all the required fields"));
			return;
		}
	}
	else {
		if (ui->lineEdit_regUsr->text().isEmpty() || ui->lineEdit_regPsw->text().isEmpty() || ui->lineEdit_regPswConf->text().isEmpty()) {
			incorrectOperation(tr("Please fill all the required fields"));
			return;
		}

		//Check if passwords match
		if (ui->lineEdit_regPsw->text() != ui->lineEdit_regPswConf->text()) {
			incorrectOperation(tr("Passwords does not match"));
			return;
		}
	}

	//Show loading splash screen
	mngr.showLoadingScreen(loading, tr("Connecting to server..."));

	//Emit connection signal to server
	emit(connectToServer(serverIP, serverPort.toShort()));
}

void LandingPage::radioButtonPressed()
{
	if (ui->radioButton_customAvatar->isChecked()) {
		ui->pushButton_browse->setEnabled(true);
	}
	else
	{
		ui->pushButton_browse->setEnabled(false);
		ui->label_incorrect_operation->setText("");
		ui->label_imageSize->setText("");

		//Load default profile picture
		QPixmap default(rsrcPath + "/misc/defaultProfile.png");
		ui->label_UsrIcon->setPixmap(default.scaled(ui->label_UsrIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
}


void LandingPage::connectionEstabilished()
{
	//Based on what screen we are we proceed to Login/Register
	switch (ui->tabWidget->currentIndex()) {
	case 0:
		Login();
		break;
	case 1:
		Register();
		break;
	}
}


void LandingPage::Login()
{
	//Get data from fields
	QString username = ui->lineEdit_usr->text();
	QString password = ui->lineEdit_psw->text();

	//Save login info to file
	saveUserLoginInfo(username);

	//Emit login signal to server
	emit(serverLogin(username, password));
}


void LandingPage::Register()
{
	//Get data from fields
	QString nickname = ui->lineEdit_regNick->text();
	QString username = ui->lineEdit_regUsr->text();
	QString password = ui->lineEdit_regPsw->text();
	QString passwordConf = ui->lineEdit_regPswConf->text();
	QImage userIcon = ui->label_UsrIcon->pixmap()->toImage();

	//Save login info to file
	saveUserLoginInfo(username);

	//Emit register signal to server
	emit serverRegister(username, password, nickname, userIcon);
}


void LandingPage::LoginSuccessful(User* user)
{
	mngr.hideLoadingScreen(loading);

	//Sets user
	_user = user;

	//Reset all fields in landing page
	resetFields();

	//Setup user file list
	setupFileList();

	//Updates GUI user infos (user/nick/pic)
	updateUserInfo();

	//Open logged page
	ui->stackedWidget->setCurrentIndex(1);
	ui->stackedWidget->show();
}


/******************* USER INFOS *******************/
/*
*	Update GUI user info
*	Setup user file list
*/

void LandingPage::updateUserInfo()
{
	//Set username and nickname (if present)
	if (_user->getNickname().isEmpty()) {
		//Set nickname label text
		ui->label_userNick->setText(_user->getUsername());

		//Setup username#userId text
		QString username;
		username = "#" + QString::number(_user->getUserId()).rightJustified(4, '0');
		ui->label_userUsername->setText(username);
	}
	else {
		ui->label_userNick->setText(_user->getNickname());

		QString username;
		username = _user->getUsername() + "#" + QString::number(_user->getUserId()).rightJustified(4, '0');
		ui->label_userUsername->setText(username);
	}



	//Set user profile picture
	QPixmap userPix;
	userPix.convertFromImage(_user->getIcon());

	setupUserProfilePicture(userPix.scaled(ui->label_userProfilePhoto->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void LandingPage::setupFileList()
{
	//Get all documents
	QList<URI> documents = _user->getDocuments();

	//Clear document list
	ui->tableWidget->clear();

	//Reset row/column count
	ui->tableWidget->setRowCount(0);


	//If user has no documents
	if (documents.isEmpty()) {
		//Hide table headers
		ui->tableWidget->horizontalHeader()->setVisible(false);
		ui->tableWidget->verticalHeader()->setVisible(false);

		//Set only one column to contain "<No documents found>"
		ui->tableWidget->setColumnCount(1);

		//Insert new row
		ui->tableWidget->insertRow(0);

		//Create "<No documents found>" element
		QTableWidgetItem* noDocuments = new QTableWidgetItem("<No documents found>");

		//Setting item style
		noDocuments->setTextAlignment(Qt::AlignCenter);
		noDocuments->setFont(QFont("Helvetica", 12));
		noDocuments->setTextColor(Qt::darkGray);
		noDocuments->setFlags(noDocuments->flags() & ~Qt::ItemIsSelectable);

		//Set item in the table
		ui->tableWidget->setItem(0, 0, noDocuments);
	}
	else
	{
		//Show table headers
		ui->tableWidget->horizontalHeader()->setVisible(true);
		ui->tableWidget->verticalHeader()->setVisible(true);

		//Set column count to 3 (Name/Author/URI)
		ui->tableWidget->setColumnCount(3);

		//Generating horizontal header
		QStringList headerLabels;
		headerLabels.append("Name");
		headerLabels.append("Author");
		headerLabels.append("URI");
		ui->tableWidget->setHorizontalHeaderLabels(headerLabels);


		//Add every document to the table
		QList<URI>::iterator it;
		for (it = documents.begin(); it != documents.end(); it++) {

			//Inserting new row in table
			int rowCount = ui->tableWidget->rowCount();
			ui->tableWidget->insertRow(rowCount);

			//Add items in table
			ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(it->getDocumentName()));		//Document name
			ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(it->getAuthorName()));		//Document author
			ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(it->toString()));			//Document URI
			ui->tableWidget->item(rowCount, 0)->setIcon(QIcon(rsrcPath + "/landingPage/richtext.png"));	//Icon
		}
	}

	//Set default behaviour of theese push buttons (disabled)
	ui->pushButton_remove->setEnabled(false);
	ui->pushButton_open->setEnabled(false);
}


/******************* GUI *******************/
/*
*	Reset fields
*	Switch between login/register pushbutton text
*	Error messages
*	Enable pushbutton Open/Remove according to documents in list
*	Close all windows
*	User profile picture preview
*/

void LandingPage::setupUserProfilePicture(QPixmap userPix)
{
	//Create a base pixmap
	QPixmap base(ui->label_userProfilePhoto->size());
	base.fill(Qt::transparent);

	//Initialize painter to create a rounded profile picture
	QPainter painter(&base);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	//Create a round shape to set label rounded
	QPainterPath path;
	path.addRoundedRect(0, 0, ui->label_userProfilePhoto->width(), ui->label_userProfilePhoto->height(), ui->label_userProfilePhoto->width() / 2, ui->label_userProfilePhoto->height() / 2);
	painter.setClipPath(path);

	//Draw profile picture in the new shape
	painter.drawPixmap(0, 0, userPix);
	ui->label_userProfilePhoto->setPixmap(base);
}

void LandingPage::resetFields()
{
	//Reset all UI fields
	ui->lineEdit_psw->setText("");
	ui->lineEdit_regUsr->setText("");
	ui->lineEdit_regNick->setText("");
	ui->lineEdit_regPsw->setText("");
	ui->lineEdit_regPswConf->setText("");
	//ui->lineEdit_UsrIconPath->setText("");
	ui->label_incorrect_operation->setText("");
	ui->label_incorrect_file_operation->setText("");

	//Reset also additional windows fields
	newFileWindow->resetFields();
	openURIWindow->resetFields();
}


void LandingPage::currentTabChanged(int index)
{
	//Reset all fields
	resetFields();

	//Change push button text according to tab
	switch (index) {
	case 0:
		ui->pushButton_confirmOperation->setText(tr("Login"));
		break;
	case 1:
		ui->pushButton_confirmOperation->setText(tr("Register"));
		break;
	}

}


void LandingPage::incorrectFileOperation(QString error)
{
	mngr.hideLoadingScreen(loading);

	//Print errors
	//openURIWindow->incorrectOperation(error);
	//newFileWindow->incorrectOperation(error);
	ui->label_incorrect_file_operation->setText(error);

	//Recompute file list
	setupFileList();
}


void LandingPage::incorrectOperation(QString error)
{
	mngr.hideLoadingScreen(loading);

	//Set Home Page error
	ui->label_incorrect_operation->setText(error);
}


void LandingPage::documentDismissed()
{
	mngr.hideLoadingScreen(loading);

	//Reset error messages
	resetFields();

	//Recompute file list
	setupFileList();
}


void LandingPage::enablePushButtonOpen()
{
	//Disables push buttons if document list is empty
	if (ui->tableWidget->item(0, 0)->text() != "<No documents found>")
	{
		ui->pushButton_open->setEnabled(true);
		ui->pushButton_remove->setEnabled(true);
	}
	else {
		ui->pushButton_open->setEnabled(false);
		ui->pushButton_remove->setEnabled(false);
	}

}


void LandingPage::closeAll()
{
	mngr.hideLoadingScreen(loading);

	//Reset error messages
	resetFields();

	//Close all LandingPage
	openURIWindow->close();
	newFileWindow->close();
	this->hide();
}


void LandingPage::updateUserAvatarPreview(QString path)
{
	QFileInfo file(path);

	int w = ui->label_UsrIcon->width();
	int h = ui->label_UsrIcon->height();

	//Check if file in path exist and if it is a valid image file
	if (file.exists() && file.isFile()) {
		QPixmap userPix(path);
		if (userPix.isNull()) {
			//Shows error
			incorrectOperation(tr("Please choose a valid image file"));
			ui->label_imageSize->setText("");
		}
		else
		{
			qint64 fileSize = file.size();
			ui->label_imageSize->setText("Image size: " + QString::number(fileSize / 1024) + " KB");

			if (fileSize > 1048576) {
				//Shows error
				incorrectOperation(tr("Choosen image is too big, please select another one"));
			}
			else {
				ui->label_UsrIcon->setPixmap(userPix.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
				ui->label_incorrect_operation->setText("");
				return;
			}
		}
	}


	//Load default profile picture
	QPixmap default(rsrcPath + "/misc/defaultProfile.png");
	ui->label_UsrIcon->setPixmap(default.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


/******************* PUSH BUTTONS *******************/
/*
*	Register
*	Browse (profile picture)
*	Open/Remove document
*	Open from URI
*	New File
*	Logout
*/


void LandingPage::pushButtonRegisterClicked()
{
	//Switch to register page
	ui->tabWidget->setCurrentIndex(1);
}


void LandingPage::pushButtonBrowseClicked()
{
	//Open file browser dialog to choose profile picture
	QString filename = QFileDialog::getOpenFileName(this, "Choose your profile icon",
		QDir::homePath(), "Image files(*.png *.jpg *.bmp)");

	if (!filename.isEmpty()) {
		//Set choosen filename
		updateUserAvatarPreview(filename);
	}
}


void LandingPage::pushButtonOpenClicked()
{
	//Open only if is not "<No documents found>" element
	QString fileSelected = ui->tableWidget->currentItem()->text();
	if (fileSelected != "<No documents found>") {
		mngr.showLoadingScreen(loading, tr("Opening document..."));

		//Send request to open file to server
		emit openDocument(_user->getURIat(ui->tableWidget->currentRow()));
	}

}

void LandingPage::pushButtonRemoveClicked()
{
	//Remove only if is not "<No documetts found>" element
	QString fileSelected = ui->tableWidget->currentItem()->text();
	if (fileSelected != "<No documents found>") {
		mngr.showLoadingScreen(loading, tr("Removing document..."));

		//Send request to remove file to server
		emit removeDocument(_user->getURIat(ui->tableWidget->currentRow()));
	}
}


void LandingPage::pushButtonOpenUriClicked()
{
	//Show and lanch dialog
	if (openURIWindow->exec() == QDialog::Accepted) {
		mngr.showLoadingScreen(loading, tr("Open document from URI..."));

		//Adds document recived from open uri
		emit openDocument(URI(_buffer));
	};

	//Reset all fields of openURIWindow
	openURIWindow->resetFields();
}

void LandingPage::pushButtonNewClicked()
{
	//Show and launch dialog
	if (newFileWindow->exec() == QDialog::Accepted) {
		mngr.showLoadingScreen(loading, tr("Creating new document..."));

		//Adds document recived from open uri
		emit newDocument(_buffer);
	};

	//Reset all fields of newFileWindow
	newFileWindow->resetFields();
}


void LandingPage::pushButtonBackClicked()
{
	//Reset fields
	resetFields();

	//Close all windows
	openURIWindow->close();
	newFileWindow->close();

	//Return to Home Page
	ui->stackedWidget->setCurrentIndex(0);
	ui->tabWidget->setCurrentIndex(0);
	ui->stackedWidget->show();

	//Set focus to correct field
	ui->lineEdit_psw->setFocus();
}


/******************* USER LOGIN INFO *******************/
/*
*	Save/Load infos (ip/port/username)
*/

void LandingPage::loadUserLoginInfo()
{
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
}


void LandingPage::saveUserLoginInfo(QString username)
{
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
}
