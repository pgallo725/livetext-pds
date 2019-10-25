#include <QApplication>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "LiveText.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(textedit); //Inizializza .qrc per il bind delle immagini

	//Crea applicazione con vari parametri
	QApplication a(argc, argv); 
	QCoreApplication::setApplicationName("LiveText");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	LiveText liveText;

	liveText.launch();

	//Entra nel loop principale dell'applicazione in attesa di azioni e attende fino alla exit (chiusura app)
	return a.exec();
}