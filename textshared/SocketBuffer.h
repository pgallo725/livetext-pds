#pragma once

#include <QObject>
#include <QByteArray>

class SocketBuffer
{
	friend class TcpServer;
	friend class WorkSpace;
private:
	quint16 type;
	quint32 size;
	QByteArray buffer;

public:
	SocketBuffer();
	~SocketBuffer();

	/* setter */
	void setType(quint16 t);
	void setDataSize(quint32 s);
	
	/* getter */
	quint16 getType();
	quint32 getDataSize();
	quint32 getReadDataSize();
	void append(QByteArray array);
	void clear();
	bool bufferReadyRead();
};

