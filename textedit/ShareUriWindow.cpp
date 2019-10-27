#include "ShareUriWindow.h"
#include "ui_shareuriwindow.h"

#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QClipboard>

#include "WidgetsManager.h"

const QString rsrcPath = ":/images";

ShareUriWindow::ShareUriWindow(QString uri, QStatusBar* statusBar, QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ShareUriWindow), _statusBar(statusBar), _uri(uri) {
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

void ShareUriWindow::copyAndClose()
{
	//Copy uri to clip board
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(_uri);

	//Close window
	this->close();

	//Show message to clipboard
	_statusBar->showMessage(tr("URI copied into clipboards"), 2000);
}
