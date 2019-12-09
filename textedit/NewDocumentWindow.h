#ifndef NEWFILEWINDOW_H
#define NEWFILEWINDOW_H

#include <QDialog>

namespace Ui { class NewDocumentWindow; }

class NewDocumentWindow : public QDialog
{
	Q_OBJECT

private:

	//Show an error
	void incorrectOperation(QString error);

public:

	Ui::NewDocumentWindow* ui;
	NewDocumentWindow(QWidget* parent = nullptr);
	~NewDocumentWindow();

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


#endif // NEWFILEWINDOW_H