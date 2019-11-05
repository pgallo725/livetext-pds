#include "AboutWindow.h"
#include "ui_aboutwindow.h"

#include <QDesktopServices>
#include <QUrl>

#include "WidgetsManager.h"

const QString rsrcPath = ":/images";

AboutWindow::AboutWindow(QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::AboutWindow){
	//Window name
	setWindowTitle(tr("About"));

	//UI setup
	ui->setupUi(this);

	//Center and resize window
	WidgetsManager mngr(this);
	mngr.centerAndResize(0.4, 0.3);

	//Setup label logo
	QPixmap logoPix(rsrcPath + "/misc/logo.png");
	ui->label_logo->setPixmap(logoPix.scaled(ui->label_logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

AboutWindow::~AboutWindow()
{
}