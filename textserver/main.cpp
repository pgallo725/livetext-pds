#include <QCoreApplication>

#include <QSaveFile>

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
		qCritical().nospace() << endl << "(ERROR) " << ex.what() << endl;
		return EXIT_FAILURE;
	}

}
