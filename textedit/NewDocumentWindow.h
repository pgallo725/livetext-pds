#ifndef NEWFILEWINDOW_H
#define NEWFILEWINDOW_H

#include <QDialog>

namespace Ui { class NewDocumentWindow; }

class NewDocumentWindow : public QDialog
{
	Q_OBJECT

private:
	// Application buffer which holds the name of the document to be opened
	QString& _filename;

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
};


#endif // NEWFILEWINDOW_H