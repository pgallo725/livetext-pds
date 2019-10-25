#ifndef SHAREURIWINDOW_H
#define SHAREURIWINDOW_H

#include <QDialog>
#include <QStatusBar>

namespace Ui {
	class ShareUriWindow;
}


class ShareUriWindow : public QDialog
{
	Q_OBJECT

private:
	QString _uri;

	//Editor status bar to print in
	QStatusBar* _statusBar;

	//Copy URI into clipboards
	void copyAndClose();

public:
	ShareUriWindow(QString uri, QStatusBar* editor, QWidget* parent = nullptr);
	~ShareUriWindow();

	Ui::ShareUriWindow* ui;
};


#endif //SHAREURIWINDOW_H