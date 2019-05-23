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

private:
	Ui::WelcomeWindow* ui;

	void openEditor();
};

#endif // MAINWINDOW_H