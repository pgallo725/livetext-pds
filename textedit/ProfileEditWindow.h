#ifndef PROFILEEDITWINDOW_H
#define PROFILEEDITWINDOW_H

#include <QDialog>
#include <User.h>

#include <QLabel>

namespace Ui {
	class ProfileEditWindow;
}

class ProfileEditWindow : public QDialog
{
	Q_OBJECT

public:
	ProfileEditWindow(User* user, QWidget* parent = nullptr);
	~ProfileEditWindow();

	void updateSuccessful();
	void updateFailed(QString error);

private slots:
	void pushButtonUpdateClicked();
	void pushButtonBrowseClicked();
	void pushButtonCancelClicked();
	void showUserIcon(QString path);

signals:
	void accountUpdate(QString name, QImage image, QString password);

private:
	QLabel* loading;

	Ui::ProfileEditWindow* ui;
	void centerAndResize();
	void startLoadingAnimation(QString text);
	void stopLoadingAnimation();

};


#endif //PROFILEEDITWINDOW_H

