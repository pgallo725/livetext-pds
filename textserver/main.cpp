#include <QCoreApplication>

#include "TcpServer.h"
#include "ServerException.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	try
	{
		TcpServer server;

		QCoreApplication::setApplicationName("LiveText - Server");
		QCoreApplication::setApplicationVersion(QT_VERSION_STR);
		
		// Possible failures in initialize are notified by throwing StartupException
		// or a sub-type of DatabaseException with more specific details
		server.initialize();

		return a.exec();
	}
	catch (ServerException& ex) 
	{
		qCritical().nospace() << endl << ex.what() << endl;

		getchar();

		return EXIT_FAILURE;
	}

}
