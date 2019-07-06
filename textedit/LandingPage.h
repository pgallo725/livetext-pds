#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

#include "Client.h"


namespace Ui {
	class LandingPage;
}

class LandingPage : public QMainWindow
{
	Q_OBJECT

public:
	LandingPage(Client* client, QWidget * parent = nullptr);
	~LandingPage();

	void openEditor(int mode, QString path = nullptr);
	enum openMode {uri, newfile};

private slots:
	void pushButtonLoginClicked();
	void pushButtonNewClicked();
	void pushButtonRegisterClicked();
	void pushButtonBrowseClicked();
	void pushButtonConfirmRegistrationClicked();
	void pushButtonBackClicked();
	void pushButtonOpenClicked();
	void pushButtonOpenUriClicked();
	void enablePushButtonOpen();



	void showUserIcon(QString path);

private:
	Ui::LandingPage* ui;
	Client* client;

private:
	void setupFileList();
	void centerAndResize();
	
};

#endif // MAINWINDOW_H