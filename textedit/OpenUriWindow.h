#ifndef OPENURIWINDOW_H
#define OPENURIWINDOW_H

#include <QDialog>

namespace Ui { class OpenUriWindow; }

class OpenUriWindow : public QDialog
{
	Q_OBJECT

private:
	// Application buffer holding the URI of the document to be opened
	QString& _uri;

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
};


#endif // OPENURIWINDOW_H