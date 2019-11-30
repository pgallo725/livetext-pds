#include "OpenUriWindow.h"
#include "ui_openuriwindow.h"

#include "WidgetsManager.h"


OpenUriWindow::OpenUriWindow(QString& uri, QWidget* parent) 
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::OpenUriWindow), _uri(uri) 
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
	//Gets URI inserted by user
	_uri = ui->lineEdit_uri->text();

	//If filename is valid it closes and confirms operation else print an error
	if (!_uri.isEmpty())
		this->done(QDialog::Accepted);
	else
		incorrectOperation(tr("Please insert a valid URI"));
}

void OpenUriWindow::rejectClicked()
{
	//Close the window
	this->close();
}
