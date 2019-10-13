#ifndef NEWFILEWINDOW_H
#define NEWFILEWINDOW_H

#include <QDialog>

class LandingPage;

namespace Ui {
	class NewFileWindow;
}

class NewFileWindow : public QDialog
{
	Q_OBJECT

public:
	Ui::NewFileWindow* ui;
	NewFileWindow(LandingPage* lp, QWidget* parent = nullptr);
	~NewFileWindow();

	void incorrectOperation(QString error);
	void resetFields();
	
private slots:
	void acceptClicked();
	void rejectClicked();
private:
	
	void centerAndResize();

	LandingPage* landingPage;

};




#endif // NEWFILEWINDOW_H