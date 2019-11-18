#include "AboutWindow.h"
#include "ui_aboutwindow.h"

#include "WidgetsManager.h"

#include <QDesktopWidget>

const QString rsrcPath = ":/images";


AboutWindow::AboutWindow(QWidget* parent) 
	: QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::AboutWindow)
{
	//Window name
	setWindowTitle(tr("About"));

	//UI setup
	ui->setupUi(this);

	//Center and resize window
	WidgetsManager mngr(this);
	//Center and resize
	if (QApplication::desktop()->availableGeometry().size().width() <= 1366)
		mngr.centerAndResize(0.45, 0.475);
	else
		mngr.centerAndResize(0.36, 0.38);
	

	//Setup label logo
	QPixmap logoPix(rsrcPath + "/misc/logo.png");
	ui->label_logo->setPixmap(logoPix.scaled(ui->label_logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

	connect(ui->pushButton, &QPushButton::clicked, this, &AboutWindow::close);
}

AboutWindow::~AboutWindow()
{
}
