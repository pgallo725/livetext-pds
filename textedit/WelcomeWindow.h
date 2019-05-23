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
	void openEditor();

private:
	Ui::WelcomeWindow* ui;

	
};

#endif // MAINWINDOW_H