#ifndef LIVETEXT_H
#define LIVETEXT_H

#include "textedit.h"
#include "LandingPage.h"
#include "Client.h"

#include <QObject>

class LiveText : public QObject
{
public:
	LiveText(QObject* parent = nullptr);
	~LiveText();

	void start();


private slots:
	//LANDING PAGE
	void openEditor(int mode, QString path);
	void connectToServer(QString ipAddress, quint16 port);
	void Login(QString username, QString password);
	void Register(QString username, QString password, QString nickname);
	void Logout();
	
	//CLIENT



	//TEXTEDIT
	void returnToLanding();
	void cursorPositionChanged(int position);

private:
	LandingPage* _landingPage;
	Client* _client;
	TextEdit* _textEdit;
};



#endif //LIVETEXT_H