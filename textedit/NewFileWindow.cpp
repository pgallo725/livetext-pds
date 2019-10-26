#include "NewFileWindow.h"
#include "ui_newfilewindow.h"

#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QRegExpValidator>

#include "WidgetsManager.h"

NewFileWindow::NewFileWindow(QString& filename, QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::NewFileWindow), _filename(filename) {
	//Window name
	setWindowTitle(tr("New file"));

	//UI setup
	ui->setupUi(this);

	//Center and resize window
	WidgetsManager mngr(this);
	mngr.centerAndResize(0.3, 0.15);

	//Connect for push buttons
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NewFileWindow::acceptClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &NewFileWindow::rejectClicked);

	//Validator to prevent insertion of '_' character in filename
	ui->lineEdit_fileName->setValidator(new QRegExpValidator(QRegExp("^[^_]+$"), this));
}

NewFileWindow::~NewFileWindow()
{
	delete ui;
}

void NewFileWindow::incorrectOperation(QString error)
{
	//Sets error
	ui->label_incorrectFilename->setText(error);
}

void NewFileWindow::resetFields()
{
	//Reset all fields
	ui->lineEdit_fileName->setText("");
	ui->label_incorrectFilename->setText("");
}

void NewFileWindow::acceptClicked()
{
	//Gets filename
	_filename = ui->lineEdit_fileName->text();

	//If filename is valid it closes and confirm operation else print an error
	if (!_filename.isEmpty())
		this->done(QDialog::Accepted);
	else
		incorrectOperation(tr("Please insert a valid filename"));
}

void NewFileWindow::rejectClicked()
{
	//Reset fields and closes
	resetFields();
	this->close();
}