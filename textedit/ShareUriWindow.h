#ifndef SHAREURIWINDOW_H
#define SHAREURIWINDOW_H

#include <QDialog>

namespace Ui {
	class ShareUriWindow;
}


class ShareUriWindow : public QDialog
{
	Q_OBJECT

private:
	QString _uri;

	//Copy URI into clipboards
	void copyAndClose();

public:
	ShareUriWindow(QString uri, QWidget* parent = nullptr);
	~ShareUriWindow();

	Ui::ShareUriWindow* ui;
};


#endif //SHAREURIWINDOW_H