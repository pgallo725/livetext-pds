#ifndef OPENURIWINDOW_H
#define OPENURIWINDOW_H

#include <QDialog>

namespace Ui {
	class OpenUriWindow;
}

class OpenUriWindow : public QDialog
{
	Q_OBJECT

public:
	OpenUriWindow(QWidget* parent = nullptr);
	~OpenUriWindow();

private slots:
	
private:
	Ui::OpenUriWindow* ui;

};




#endif // OPENURIWINDOW_H