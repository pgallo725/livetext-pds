#include <QCoreApplication>

#include "TcpServer.h"
#include "ServerException.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	try
	{
		TcpServer server;

		// Possible failures in initialize are handled by throwing StartupException
		// or a sub-type of FileException with more specific details
		server.initialize();

		return a.exec();
	}
	catch (ServerException& ex) 
	{
		qCritical().nospace() << endl << ex.what() << "\ntextserver unable to start... " << endl;
		return EXIT_FAILURE;
	}
	
}
