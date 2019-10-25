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
public:
	LiveText(QObject* parent = nullptr);
	~LiveText();

	void start();


private slots:
	//LANDING PAGE
	void openEditor();
	void connectToServer(QString ipAddress, quint16 port);
	void Login(QString username, QString password);
	void Register(QString username, QString password, QString nickname, QImage icon);
	void Logout();
	void removeDocument(int index);
	void openDocument(int index);
	void addDocument(QString uri);
	
	//TEXTEDIT
	void closeDocument();
	void closeDocumentCompleted(bool isForced);
	void sendCursor(qint32 pos);
	void sendAccountUpdate(QString nickname, QImage image, QString password);
	void accountUpdated(User user);
	void openEditProfile();

	//LIVETEXT
	void operationFailed(QString errorType);
	void loginSuccess(User user);
	void forceLogout();


	//DOCUMENTEDITOR
	void openDocumentCompleted(Document doc);
	void dismissDocumentCompleted(URI URI);

private:
	LandingPage* _landingPage;
	Client* _client;
	TextEdit* _textEdit;
	DocumentEditor* _docEditor;
	ProfileEditWindow* _editProfile;
	User _user;
};



#endif //LIVETEXT_H