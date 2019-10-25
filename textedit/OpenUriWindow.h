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
	
	OpenUriWindow(QString& uri, QWidget* parent = nullptr);
	~OpenUriWindow();
		
	//Show an error
	void incorrectOperation(QString error);

	//Reset all fields
	void resetFields();

private slots:
	//Buttons slots
	void acceptClicked();
	void rejectClicked();

private:
	QString& _uri;
};




#endif // OPENURIWINDOW_H