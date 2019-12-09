#include "OpenUriWindow.h"
#include "ui_openuriwindow.h"

#include <QRegularExpression>

#include "WidgetsManager.h"


OpenUriWindow::OpenUriWindow(QWidget* parent) 
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::OpenUriWindow)
{
	//Window name and logo
	setWindowTitle(tr("Open from URI"));

	//UI setup
	ui->setupUi(this);

	//Center and resize window
	WidgetsManager mngr(this);
	mngr.centerAndResize(0.3, 0.15);

	//Connect for push buttons
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &OpenUriWindow::acceptClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &OpenUriWindow::rejectClicked);
	
	//Set keyboard focus
	ui->lineEdit_uri->setFocus();
}

OpenUriWindow::~OpenUriWindow()
{
	delete ui;
}

void OpenUriWindow::open(QObject* receiver, const char* slot)
{
	// Connect the finished() signal to the external slot
	connect(this, SIGNAL(finished(const QString&)), receiver, slot);

	//Show the window
	((QDialog*)this)->open();
}

void OpenUriWindow::incorrectOperation(QString error)
{
	//Sets error
	ui->label_incorrectUri->setText(error);
}

void OpenUriWindow::resetFields()
{
	//Reset fields
	ui->label_incorrectUri->setText("");
	ui->lineEdit_uri->setText("");

	//Reset focus
	ui->lineEdit_uri->setFocus();
}

void OpenUriWindow::acceptClicked()
{
	QString uri = ui->lineEdit_uri->text();
	// RegExp to check if the candidate URI has the correct format
	QRegularExpression uriFormat("^[^_]+_[^_]+_[a-zA-Z0-9]{12}$");

	//If provided URI is incorrect it prints an error, otherwise close and proceed
	if (ui->lineEdit_uri->text().isEmpty())
		incorrectOperation(tr("Please insert a valid URI"));
	else if (!uriFormat.match(uri).hasMatch())	
		incorrectOperation(tr("The provided URI has an invalid format"));
	else 
	{
		emit finished(ui->lineEdit_uri->text());
		this->close();
	}
}

void OpenUriWindow::rejectClicked()
{
	//Close the window
	this->close();
}
