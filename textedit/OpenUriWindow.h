#ifndef OPENURIWINDOW_H
#define OPENURIWINDOW_H

#include <QDialog>
#include "LandingPage.h"

namespace Ui {
	class OpenUriWindow;
}

class OpenUriWindow : public QDialog
{
	Q_OBJECT

public:
	OpenUriWindow(LandingPage* lp, QWidget* parent = nullptr);
	~OpenUriWindow();

private slots:
	void acceptClicked();
	void rejectClicked();
private:
	Ui::OpenUriWindow* ui;
	void centerAndResize();

	LandingPage* landingPage;

};




#endif // OPENURIWINDOW_H