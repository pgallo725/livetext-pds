#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

namespace Ui {
	class LandingPage;
}

class LandingPage : public QMainWindow
{
	Q_OBJECT

public:
	LandingPage(QWidget * parent = nullptr);
	~LandingPage();

	void openEditor(QString path = nullptr);

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

	
	void setupFileList();
	void centerAndResize();
	
};

#endif // MAINWINDOW_H