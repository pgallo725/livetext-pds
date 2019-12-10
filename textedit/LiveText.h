#ifndef LIVETEXT_H
#define LIVETEXT_H

#include <QObject>
#include <QWaitCondition>
#include <QSharedPointer>

#include <User.h>
#include "Client.h"
#include "LandingPage.h"
#include "TextEdit.h"
#include "DocumentEditor.h"


class LiveText : public QObject
{
	Q_OBJECT

private:

	//Client
	Client* _client;

	//Current user
	User _user;

	//Landing page
	LandingPage* _landingPage;

	//Text editor
	TextEdit* _textEdit;
	bool editorOpen;

	//Local document editor
	DocumentEditor* _docEditor;

	//Sync variable
	QSharedPointer<QWaitCondition> _wc;

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
	void forceCloseDocument();
	void closeDocumentCompleted();
	void dismissDocumentCompleted(URI URI);

	//User profile editing
	void accountUpdated(User user);
	void openEditProfile(bool fromEditor);

	//Server connection
	void loginSuccess(User user);
	void logoutClient();
	void showForceLogout();
	void forceLogout();

	//GUI
	void operationFailed(QString errorType);

signals:

	void closeConnection();
	void logout();
};

#endif //LIVETEXT_H