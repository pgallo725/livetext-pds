#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class WelcomeWindow;
}

class WelcomeWindow : public QMainWindow
{
	Q_OBJECT

public:
	WelcomeWindow(QWidget * parent = nullptr);
	~WelcomeWindow();

private slots:
	void pushButtonLoginClicked();
	void pushButtonNewClicked();
	void pushButtonRegisterClicked();
	void pushButtonBrowseClicked();
	void pushButtonConfirmRegistrationClicked();
	void pushButtonBackClicked();
	void pushButtonOpenClicked();


	void showUserIcon(QString path);

private:
	Ui::WelcomeWindow* ui;
	void openEditor(QString path = nullptr);
	void setupFileList();
	void centerAndResize();
	
};

#endif // MAINWINDOW_H