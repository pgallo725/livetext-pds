#include "NewFileWindow.h"
#include "ui_newfilewindow.h"


#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

NewFileWindow::NewFileWindow(LandingPage* lp, QWidget* parent) : landingPage(lp), QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::NewFileWindow) {
	//Costruttore landing page
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
	centerAndResize();
	
	//Imposto la label "Incorrect Filename" non visibile
	ui->label_incorrectFilename->setVisible(false);

	//Connect con i bottoni del dialog box
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NewFileWindow::acceptClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NewFileWindow::rejectClicked);

	//Inizializzo il comboBox
	ui->comboBox_fileType->addItem("rtf");
	ui->comboBox_fileType->addItem("txt");
}

NewFileWindow::~NewFileWindow()
{
	delete ui;
}

void NewFileWindow::acceptClicked()
{
	QString name = ui->lineEdit_fileName->text();
	QString extension = ui->comboBox_fileType->currentText();

	bool notvalid = false;

	if (name == "" || notvalid) {
		ui->label_incorrectFilename->setVisible(true);
	}
	else {
		QString filename = name + "." + extension;

		this->close();
		landingPage->openEditor(landingPage->newfile, filename);
	}
}

void NewFileWindow::rejectClicked()
{
	this->close();
}

void NewFileWindow::centerAndResize() {
	//Ricava dimensione desktop
	QSize availableSize = QApplication::desktop()->availableGeometry().size();
	int width = availableSize.width();
	int height = availableSize.height();

	//Proporzionamento
	width *= 0.3;
	height *= 0.15;

	//Le dimensioni vengono fissate per rendere la finestra non resizable
	setMaximumHeight(height);
	setMinimumHeight(height);
	setMaximumWidth(width);
	setMinimumWidth(width);

	//Nuova dimensione
	QSize newSize(width, height);

	//Crea il nuovo rettangolo su cui aprire la finestra
	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, newSize, QApplication::desktop()->availableGeometry()));
}
