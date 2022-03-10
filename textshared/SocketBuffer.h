#pragma once
#ifndef _SOCKET_BUFFER_
#define _SOCKET_BUFFER_

#include <QByteArray>
#include <QDataStream>

class SocketBuffer
{
	/* Operators for QDataStream deserialization */
	friend QDataStream& operator>>(QDataStream& in, SocketBuffer& socketBuffer);		// Input

private:
	quint16 mType;
	quint32 mSize;
	QByteArray buffer;

public:
	SocketBuffer();
	~SocketBuffer();

	void append(QByteArray array);
	void clearBuffer();
	bool bufferReadyRead();
	QByteArray* bufferPtr();

	/* setter */
	void setType(quint16 t);
	void setDataSize(quint32 s);
	
	/* getter */
	quint16 getType() const;
	quint32 getDataSize() const;
	quint32 getReadDataSize() const;
};

#endif // _SOCKET_BUFFER_
