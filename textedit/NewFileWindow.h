#ifndef NEWFILEWINDOW_H
#define NEWFILEWINDOW_H

#include <QDialog>
#include "LandingPage.h"

namespace Ui {
	class NewFileWindow;
}

class NewFileWindow : public QDialog
{
	Q_OBJECT

public:
	NewFileWindow(LandingPage* lp, QWidget* parent = nullptr);
	~NewFileWindow();
	Ui::NewFileWindow* ui;
private slots:
	void acceptClicked();
	void rejectClicked();
private:
	
	void centerAndResize();

	LandingPage* landingPage;

};




#endif // NEWFILEWINDOW_H