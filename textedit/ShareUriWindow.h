#ifndef SHAREURIWINDOW_H
#define SHAREURIWINDOW_H
#include <QDialog>


namespace Ui {
	class ShareUriWindow;
}


class ShareUriWindow : public QDialog
{
	Q_OBJECT

public:
	ShareUriWindow(QString text, QWidget* parent = nullptr);
	~ShareUriWindow();
	Ui::ShareUriWindow* ui;

private:
	void centerAndResize();


};




#endif //SHAREURIWINDOW_H