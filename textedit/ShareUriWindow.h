#ifndef SHAREURIWINDOW_H
#define SHAREURIWINDOW_H

#include <QDialog>

namespace Ui { class ShareUriWindow; }

class ShareUriWindow : public QDialog
{
	Q_OBJECT

private:
	//Copy URI into clipboards
	void copyAndClose();

public:

	Ui::ShareUriWindow* ui;
	ShareUriWindow(QString uri, QWidget* parent = nullptr);
	~ShareUriWindow();
};


#endif //SHAREURIWINDOW_H