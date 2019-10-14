#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <Document.h>

#include "OpenUriWindow.h"
#include "NewFileWindow.h"
#include "Client.h"


namespace Ui {
	class LandingPage;
}

class LandingPage : public QMainWindow
{
	Q_OBJECT

public:
	LandingPage(QWidget* parent = nullptr);
	~LandingPage();

	void openLoggedPage();
	void incorrectOperation(QString msg);
	void documentDismissed(); //Remove document
	void setupFileList(); //Document List
	void closeAll();

	void startLoadingAnimation(QString text);
	void stopLoadingAnimation();

	void loadUserLoginInfo();
	void saveUserLoginInfo(QString username);
	void setUser(User* user);
	void updateUserInfo();
	void resetFields();

private slots:
	void pushButtonNewClicked();
	void pushButtonRegisterClicked();
	void pushButtonBrowseClicked();
	void pushButtonOpenClicked();
	void pushButtonRemoveClicked();
	void pushButtonOpenUriClicked();
	void enablePushButtonOpen();
	void currentTabChanged(int index);
	void showUserIcon(QString path);
	void confirmOperation();


public slots:
	void connectionEstabilished();
	void impossibleToConnect();
	void incorrectFileOperation(QString error);
	void pushButtonBackClicked();

signals:
	void newDocument(QString name);
	void addDocument(QString URI);
	void openDocument(int index);
	void removeDocument(int index);
	void connectToServer(QString ipAddress, quint16 port);
	void serverLogin(QString username, QString password);
	void serverRegister(QString username, QString password, QString nickname, QImage icon);
	void serverLogout();
	void editProfile();

private:
	Ui::LandingPage* ui;
	Client* client;
	User* _user;
	QLabel* loading;

	OpenUriWindow* openURIWindow;
	NewFileWindow* newFileWindow;

private:
	void centerAndResize();

	void Register();
	void Login();

};

#endif // LANDINGPAGE_H