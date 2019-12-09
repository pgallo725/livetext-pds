#include "NewDocumentWindow.h"
#include "ui_newdocumentwindow.h"

#include <QRegExpValidator>

#include "WidgetsManager.h"
#include "Document.h"


NewDocumentWindow::NewDocumentWindow(QWidget* parent) 
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::NewDocumentWindow)
{
	//Window name
	setWindowTitle(tr("New document"));

	//UI setup
	ui->setupUi(this);

	//Center and resize window
	WidgetsManager mngr(this);
	mngr.centerAndResize(0.3, 0.15);

	//Connect for push buttons
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NewDocumentWindow::acceptClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NewDocumentWindow::rejectClicked);

	//Validator to prevent insertion of '_' character in filename
	ui->lineEdit_fileName->setValidator(new QRegExpValidator(QRegExp(QString("^[^") + URI_FIELD_SEPARATOR + "]+$"), this));

	//Set keyboard focus
	ui->lineEdit_fileName->setFocus();
}

NewDocumentWindow::~NewDocumentWindow()
{
	delete ui;
}

void NewDocumentWindow::open(QObject* receiver, const char* slot)
{
	// Connect the finished() signal to the external slot
	connect(this, SIGNAL(finished(const QString&)), receiver, slot);

	//Show the window
	((QDialog*)this)->open();
}

void NewDocumentWindow::incorrectOperation(QString error)
{
	//Sets error
	ui->label_incorrectFilename->setText(error);
}

void NewDocumentWindow::resetFields()
{
	//Reset all fields
	ui->lineEdit_fileName->setText("");
	ui->label_incorrectFilename->setText("");

	//Reset the focus
	ui->lineEdit_fileName->setFocus();
}

void NewDocumentWindow::acceptClicked()
{
	//If filename is valid it closes and confirm operation else print an error
	if (ui->lineEdit_fileName->text().isEmpty())
		incorrectOperation(tr("Please insert a valid document name"));
	else
	{
		emit finished(ui->lineEdit_fileName->text());
		this->close();
	}
}

void NewDocumentWindow::rejectClicked()
{
	//Close the window
	this->close();
}
