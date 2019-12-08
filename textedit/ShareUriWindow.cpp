#include "ShareUriWindow.h"
#include "ui_shareuriwindow.h"

#include <QClipboard>

#include "WidgetsManager.h"

const QString rsrcPath = ":/images";


ShareUriWindow::ShareUriWindow(QString uri, QWidget* parent) 
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ShareUriWindow) 
{
	//Setup UI window and resize
	ui->setupUi(this);

	//Widget size manager
	WidgetsManager mngr(this);
	mngr.centerAndResize(0.3, 0.12);

	//Set uri in textbox
	ui->lineEdit_uri->setText(uri);

	//Copy icon
	ui->pushButton_copy->setIcon(QIcon::QIcon(rsrcPath + "/editor/editcopy.png"));

	//Connect pushbutton
	connect(ui->pushButton_copy, &QPushButton::clicked, this, &ShareUriWindow::copyAndClose);
}


ShareUriWindow::~ShareUriWindow()
{
	delete ui;
}

void ShareUriWindow::open(QObject* receiver, const char* slot)
{
	// Connect the finished() signal to the notification slot
	connect(this, SIGNAL(finished(const QString&)), receiver, slot);

	//Show the window
	((QDialog*)this)->open();
}

void ShareUriWindow::copyAndClose()
{
	//Copy uri to clipboard
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(ui->lineEdit_uri->text());

	//Close window
	emit finished(tr("URI copied into clipboards"));
	this->close();
}
