#include <QCoreApplication>

#include <iostream>
#include "TcpServer.h"

#include "ServerException.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	try
	{
		TcpServer server;

		server.initialize();	// may throw FileLoadException

		return a.exec();
	}
	catch (ServerStartException& e) 
	{
		std::cerr << '\n' << e.what() << "\ntextserver unable to start ... " << std::endl;
		return EXIT_FAILURE;
	}
	catch (FileException &fe)
	{
		std::cerr << '\n' << fe.what() << "\ntextserver initialization failed\n" << std::endl;
		return EXIT_FAILURE;
	}
	
}
