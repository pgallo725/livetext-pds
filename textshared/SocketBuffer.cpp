#include "SocketBuffer.h"


SocketBuffer::SocketBuffer() 
	: mType(0), mSize(0)
{
};

SocketBuffer::~SocketBuffer() 
{
};

void SocketBuffer::setType(quint16 t) 
{ 
	mType = t;
};

void SocketBuffer::setDataSize(quint32 s) 
{ 
	mSize = s;
};

quint16 SocketBuffer::getType() const
{ 
	return mType;
};

quint32 SocketBuffer::getDataSize() const
{
	return mSize; 
};

quint32 SocketBuffer::getReadDataSize() const
{ 
	return (quint32)buffer.size(); 
};

void SocketBuffer::append(QByteArray array) 
{ 
	buffer.append(array); 
};

void SocketBuffer::clear() 
{ 
	mSize = 0; buffer.clear(); 
};

bool SocketBuffer::bufferReadyRead() 
{ 
	return mSize == getReadDataSize(); 
};

QDataStream& operator>>(QDataStream& in, SocketBuffer& socketBuffer)
{
	in >> socketBuffer.mType >> socketBuffer.mSize;

	return in;
}
