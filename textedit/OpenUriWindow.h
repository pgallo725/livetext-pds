#ifndef OPENURIWINDOW_H
#define OPENURIWINDOW_H

#include <QDialog>

class LandingPage;

namespace Ui {
	class OpenUriWindow;
}

class OpenUriWindow : public QDialog
{
	Q_OBJECT

public:
	Ui::OpenUriWindow* ui;
	
	OpenUriWindow(LandingPage* lp, QWidget* parent = nullptr);
	~OpenUriWindow();
		
	void incorrectOperation(QString error);
	void resetFields();
	
private slots:
	void acceptClicked();
	void rejectClicked();
private:
	
	void centerAndResize();

	LandingPage* landingPage;

};




#endif // OPENURIWINDOW_H