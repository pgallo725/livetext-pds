#ifndef PROFILEEDITWINDOW_H
#define PROFILEEDITWINDOW_H

#include <QDialog>
#include <QLabel>
#include <User.h>

#include "WidgetsManager.h"

namespace Ui { class ProfileEditWindow; }

class ProfileEditWindow : public QDialog
{
	Q_OBJECT

private:

	//Reference to user object
	User& _user;

	Ui::ProfileEditWindow* ui;

	//Widget manager (resize/loading screen)
	WidgetsManager mngr;

	//Loading splash screen
	QLabel* loading;

	//Checks if window is open from editor (true) or from LandingPage (false)
	bool _editorFlag;

	//Flag used to determine whether the image was changed or not
	bool _iconChanged;


	//Methods
	void resetFields();
	void updateInfo();
	
public:

	ProfileEditWindow(User& user, bool fromEditor, QWidget* parent = nullptr);
	~ProfileEditWindow();

public slots:

	//REMOTE: response from server
	void updateSuccessful();
	void updateFailed(QString error);

private slots:
	//Push buttons
	void pushButtonUpdateClicked();
	void pushButtonBrowseClicked();

	//Password editing
	void passwordEdited();

	//Radio button
	void radioButtonPressed();

	//GUI update
	void updateSelectedAvatar(const QString& path);

signals:

	//LOCAL: Account modified
	void accountUpdate(QString name, QImage image, QString password, bool fromEditor);
};


#endif //PROFILEEDITWINDOW_H
