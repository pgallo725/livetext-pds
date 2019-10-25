#ifndef PROFILEEDITWINDOW_H
#define PROFILEEDITWINDOW_H

#include <QDialog>
#include <User.h>

#include <QLabel>

#include "WidgetsManager.h"

namespace Ui {
	class ProfileEditWindow;
}

class ProfileEditWindow : public QDialog
{
	Q_OBJECT

private:
	//Loading splash screen
	QLabel* loading;

	//User infos
	User& _user;

	//Widget manager (resize/loading screen)
	WidgetsManager mngr;

	Ui::ProfileEditWindow* ui;

	//Methods
	void resetFields();
	

public:
	ProfileEditWindow(User& user, QWidget* parent = nullptr);
	~ProfileEditWindow();

	//REMOTE: response from server
	void updateSuccessful();
	void updateFailed(QString error);
	void updateInfo();

signals:
	//LOCAL: Account modified
	void accountUpdate(QString name, QImage image, QString password);

private slots:
	//Push buttons
	void pushButtonUpdateClicked();
	void pushButtonBrowseClicked();
	void pushButtonCancelClicked();

	//GUI update
	void showUserIcon(QString path);
};


#endif //PROFILEEDITWINDOW_H

