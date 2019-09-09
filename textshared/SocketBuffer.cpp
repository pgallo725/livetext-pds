#include "SocketBuffer.h"

SocketBuffer::SocketBuffer() : type(0), size(0)
{
};

SocketBuffer::~SocketBuffer() 
{
};

void SocketBuffer::setType(quint16 t) 
{ 
	type = t;
};

void SocketBuffer::setDataSize(quint32 s) 
{ 
	size = s;
};

quint16 SocketBuffer::getType()
{ 
	return type; 
};

quint32 SocketBuffer::getDataSize() 
{
	return size; 
};

quint32 SocketBuffer::getReadDataSize() 
{ 
	return (quint32)buffer.size(); 
};

void SocketBuffer::append(QByteArray array) 
{ 
	buffer.append(array); 
};

void SocketBuffer::clear() 
{ 
	size = 0; buffer.clear(); 
};

bool SocketBuffer::bufferReadyRead() 
{ 
	return size == getReadDataSize(); 
};