#ifndef NEWFILEWINDOW_H
#define NEWFILEWINDOW_H

#include <QDialog>

class LandingPage;

namespace Ui {
	class NewDocumentWindow;
}

class NewDocumentWindow : public QDialog
{
	Q_OBJECT

public:
	Ui::NewDocumentWindow* ui;
	NewDocumentWindow(QString& filename, QWidget* parent = nullptr);
	~NewDocumentWindow();

	//Show an error
	void incorrectOperation(QString error);

	//Reset all fields
	void resetFields();
	
private slots:
	//Buttons slots
	void acceptClicked();
	void rejectClicked();

private:
	QString& _filename;
};




#endif // NEWFILEWINDOW_H