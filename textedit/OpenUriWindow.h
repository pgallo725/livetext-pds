#ifndef OPENURIWINDOW_H
#define OPENURIWINDOW_H

#include <QDialog>

namespace Ui { class OpenUriWindow; }

class OpenUriWindow : public QDialog
{
	Q_OBJECT

private:

	//Show an error
	void incorrectOperation(QString error);

public:

	Ui::OpenUriWindow* ui;
	OpenUriWindow(QWidget* parent = nullptr);
	~OpenUriWindow();
		
	//Open the dialog
	void open(QObject* receiver, const char* slot);

	//Reset all fields
	void resetFields();

private slots:
	//Buttons slots
	void acceptClicked();
	void rejectClicked();

signals:

	void finished(const QString& name);
};


#endif // OPENURIWINDOW_H