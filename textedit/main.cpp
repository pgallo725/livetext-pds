#include "textedit.h"
#include "WelcomeWindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QCommandLineOption>


int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(textedit); //Inizializza .qrc per il bind delle immagini

	//Crea applicazione con vari parametri
	QApplication a(argc, argv); 
	QCoreApplication::setOrganizationName("DC");
	QCoreApplication::setApplicationName("Editor di testo");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	
	WelcomeWindow mw;
	
	//Mostra la finestra di mw formata
	mw.show();

	//Entra nel loop principale dell'applicazione in attesa di azioni e attende fino alla exit (chiusura app)
	return a.exec();
}