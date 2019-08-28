#ifndef PROFILEEDITWINDOW_H
#define PROFILEEDITWINDOW_H

#include <QDialog>
#include <User.h>

namespace Ui {
	class ProfileEditWindow;
}

class ProfileEditWindow : public QDialog
{
	Q_OBJECT

public:
	ProfileEditWindow(User* user, QWidget* parent = nullptr);
	~ProfileEditWindow();

private slots:
	void pushButtonUpdateClicked();
	void pushButtonBrowseClicked();
	void pushButtonCancelClicked();
	void showUserIcon(QString path);

signals:
	void accountUpdate(QString nickname, QImage icon);

private:
	Ui::ProfileEditWindow* ui;
	void centerAndResize();

};


#endif //PROFILEEDITWINDOW_H

