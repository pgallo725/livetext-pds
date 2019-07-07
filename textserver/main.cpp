#include <QCoreApplication>

#include <iostream>
#include "TcpServer.h"

#include "ServerException.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	TcpServer server;
	
	try
	{
		server.initialize();	// may throw FileLoadException
	}
	catch (FileException &fe)
	{
		std::cerr << '\n' << fe.what() << "\ntextserver initialization failed\n" << std::endl;
		return EXIT_FAILURE;
	}
	

	return a.exec();
}
