#include "OpenUriWindow.h"
#include "ui_openuriwindow.h"

#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

OpenUriWindow::OpenUriWindow(QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::OpenUriWindow) {
	//Costruttore landing page
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
}

OpenUriWindow::~OpenUriWindow()
{
	delete ui;
}
