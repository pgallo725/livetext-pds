#include "NewFileWindow.h"
#include "ui_newfilewindow.h"

#include "LandingPage.h"

#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QRegExpValidator>

NewFileWindow::NewFileWindow(LandingPage* lp, QWidget* parent) : landingPage(lp), QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::NewFileWindow) {
	//Costruttore landing page
	setWindowTitle(tr("New file"));
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
	centerAndResize();

	//Connect con i bottoni del dialog box
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NewFileWindow::acceptClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NewFileWindow::rejectClicked);

	//Validator per non inserire lettere nei campi server/port
	ui->lineEdit_fileName->setValidator(new QRegExpValidator(QRegExp("^[^_]+$"), this));
}

NewFileWindow::~NewFileWindow()
{
	delete ui;
}

void NewFileWindow::incorrectOperation(QString error)
{
	ui->label_incorrectFilename->setText(error);
}

void NewFileWindow::resetFields()
{
	ui->lineEdit_fileName->setText("");
}

void NewFileWindow::acceptClicked()
{
	QString name = ui->lineEdit_fileName->text();

	if (name.isEmpty()) {
		incorrectOperation(tr("Please insert a valid filename"));
	}
	else {
		this->close();
		landingPage->startLoadingAnimation("Creating new document...");
		emit landingPage->newDocument(name);
	}
}

void NewFileWindow::rejectClicked()
{
	ui->label_incorrectFilename->setText("");
	ui->lineEdit_fileName->setText("");
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
