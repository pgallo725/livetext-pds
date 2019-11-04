#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <Document.h>

#include <User.h>

#include "OpenUriWindow.h"
#include "NewFileWindow.h"
#include "WidgetsManager.h"


namespace Ui {
	class LandingPage;
}

class LandingPage : public QMainWindow
{
	Q_OBJECT

private:
	Ui::LandingPage* ui;

	//Current user
	User* _user;

	//Loading splash screen
	QLabel* loading;

	//Widget Manager for resizing and loading screen
	WidgetsManager mngr;

	//Dialogs to open/create a file
	OpenUriWindow* openURIWindow;
	NewFileWindow* newFileWindow;
	//Service buffer for filename or URI
	QString _buffer;

	//Register/Login methods
	void Register();
	void Login();
	void setupUserProfilePicture(QPixmap userPix);

public:
	LandingPage(QWidget* parent = nullptr);
	~LandingPage();

	//Opens document selection page
	void LoginSuccessful(User* user);

	//Updates user info in main page
	void updateUserInfo();

	//Document list update (remove/add document)
	void documentDismissed();
	void setupFileList();
	
	//Close all LandingPage windows
	void closeAll();
	
	//Reset all UI fields
	void resetFields();

	//User login info (load/save)
	void loadUserLoginInfo();
	void saveUserLoginInfo(QString username);
	
signals:
	//Create/Open/Remove document
	void newDocument(QString name);
	void openDocument(URI uri);
	void removeDocument(URI uri);

	//Server (Connection/Login/Register/Logout)
	void connectToServer(QString ipAddress, quint16 port);
	void serverLogin(QString username, QString password);
	void serverRegister(QString username, QString password, QString nickname, QImage icon);
	void serverLogout();
	
	//Open edit profile window
	void editProfile();

public slots:
	void connectionEstabilished();
	void incorrectFileOperation(QString error);
	void pushButtonBackClicked();
	void incorrectOperation(QString error);

private slots:
	//GUI push buttons slots
	void pushButtonNewClicked();
	void pushButtonRegisterClicked();
	void pushButtonBrowseClicked();
	void pushButtonOpenClicked();
	void pushButtonRemoveClicked();
	void pushButtonOpenUriClicked();
	void pushButtonConfirmOperationClicked();
	
	//Enable/Disable push button (open/remove) when a selection in list changes
	void enablePushButtonOpen();

	//tabWidget slot to switch tabs
	void currentTabChanged(int index);

	//User profile picture preview
	void showUserIcon(QString path);
};

#endif // LANDINGPAGE_H