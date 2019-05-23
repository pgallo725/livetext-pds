#pragma once
#include <qobject.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>

class TcpServer : public QObject {
	Q_OBJECT
private:
	QTcpServer* textServer;
public:
	TcpServer(QObject *parent = 0);
	~TcpServer();

public slots:
	void newClientConnection();
	void clientDisconnection();
};

