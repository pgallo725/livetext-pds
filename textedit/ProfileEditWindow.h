#ifndef PROFILEEDITWINDOW_H
#define PROFILEEDITWINDOW_H

#include <QDialog>

namespace Ui {
	class ProfileEditWindow;
}

class ProfileEditWindow : public QDialog
{
	Q_OBJECT

public:
	ProfileEditWindow(QWidget* parent = nullptr);
	~ProfileEditWindow();

private slots:
	void pushButtonUpdateClicked();
	void pushButtonBrowseClicked();
	void pushButtonCancelClicked();
	void showUserIcon(QString path);

private:
	Ui::ProfileEditWindow* ui;
	void centerAndResize();

};


#endif //PROFILEEDITWINDOW_H

