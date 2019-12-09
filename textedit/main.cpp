#include <QApplication>
#include <QDesktopWidget>

#include "LiveText.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(textedit); //Initialize .qrc for icon and resources binding

	//Create application with the proper parameters
	QApplication app(argc, argv); 
	QCoreApplication::setApplicationName("LiveText");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	LiveText liveText;

	liveText.launch();

	//Enter main application loop waiting for events to handle or exit
	return app.exec();
}