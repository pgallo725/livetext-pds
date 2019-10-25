#ifndef LIVETEXT_H
#define LIVETEXT_H

#include "textedit.h"
#include "LandingPage.h"
#include "Client.h"
#include "DocumentEditor.h"
#include "ProfileEditWindow.h"

#include <User.h>

#include <QObject>

class LiveText : public QObject
{
private:
	//Landing page
	LandingPage* _landingPage;

	//Client
	Client* _client;

	//Text editor
	TextEdit* _textEdit;
	bool editorOpen;

	//Local document editor
	DocumentEditor* _docEditor;

	//profile edit window
	ProfileEditWindow* _editProfile;

	//Current user
	User _user;


public:
	LiveText(QObject* parent = nullptr);
	~LiveText();

	//Launch the main window
	void launch();


private slots:
	//Text editor operations
	void openEditor();
	void closeEditor();

	//Document operation
	void openDocumentCompleted(Document doc);
	void closeDocumentCompleted(bool isForced);
	void dismissDocumentCompleted(URI URI);

	//User profile editing
	void accountUpdated(User user);
	void openEditProfile();

	//Server connection
	void loginSuccess(User user);
	void forceLogout();

	//GUI
	void operationFailed(QString errorType);
};

#endif //LIVETEXT_H