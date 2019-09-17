#pragma once

#include <QObject>
#include <QByteArray>
#include <QDataStream>

class SocketBuffer
{
	friend class TcpServer;
	friend class WorkSpace;
	friend class Client;

	/* Operators for QDataStream deserialization */
	friend QDataStream& operator>>(QDataStream& in, SocketBuffer& socketBuffer);		// Input

private:
	quint16 mType;
	quint32 mSize;
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

