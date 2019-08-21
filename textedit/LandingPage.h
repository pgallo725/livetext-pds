#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>

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


	enum openMode { uri, newfile };

	void openLoggedPage();
	void incorrectOperation(QString msg);

private slots:
	void pushButtonNewClicked();
	void pushButtonRegisterClicked();
	void pushButtonBrowseClicked();
	void pushButtonOpenClicked();
	void pushButtonRemoveClicked();
	void pushButtonOpenUriClicked();
	void pushButtonBackClicked();
	void enablePushButtonOpen();
	void currentTabChanged(int index);
	void showUserIcon(QString path);
	void confirmOperation();
	

public slots:
	void connectionEstabilished();
	void impossibleToConnect();

signals:
	void openEditor(int mode, QString path = nullptr);
	void removeDocument(QString path); //TODO REMOVE DOCUMENT
	void connectToServer(QString ipAddress, quint16 port);
	void serverLogin(QString username, QString password);
	void serverRegister(QString username, QString password, QString nickname);
	void serverLogout();

private:
	Ui::LandingPage* ui;
	Client* client;

	QLabel* loading;

private:
	void setupFileList();
	void centerAndResize();
	void startLoadingAnimation();
	void stopLoadingAnimation();
	void Register();
	void Login();

};

#endif // LANDINGPAGE_H