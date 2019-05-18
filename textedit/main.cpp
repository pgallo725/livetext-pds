#include "textedit.h"

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
	
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(a);

	//Crea l'oggetto TextEdit un wrapper di QTextEdit modificato per realizzare le funzioni base
	TextEdit mw;

	//Dimensione finestra
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mw);

	//Applica la dimensione al TextEdit e lo mette nella finestra corretta
	mw.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
	mw.move((availableGeometry.width() - mw.width()) / 2, (availableGeometry.height() - mw.height()) / 2);

	//Apre il file example.html se lo trova, altrimenti ne crea uno nuovo come se lo passassi da linea di comando
	if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))
		mw.fileNew();

	//Mostra la finestra di mw formata
	mw.show();

	//Entra nel loop principale dell'applicazione in attesa di azioni e attende fino alla exit (chiusura app)
	return a.exec();
}