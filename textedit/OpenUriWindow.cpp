#include "OpenUriWindow.h"
#include "ui_openuriwindow.h"

#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

#include "WidgetsManager.h"

OpenUriWindow::OpenUriWindow(QString& uri, QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::OpenUriWindow), _uri(uri) {
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
}

void OpenUriWindow::acceptClicked()
{
	//Gets uri
	_uri = ui->lineEdit_uri->text();

	//If filename is valid it closes and confirm operation else print an error
	if (!_uri.isEmpty())
		this->done(QDialog::Accepted);
	else
		incorrectOperation(tr("Please insert a valid URI"));
}

void OpenUriWindow::rejectClicked()
{
	//Reset fields and closes
	resetFields();
	this->close();
}