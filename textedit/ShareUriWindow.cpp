#include "ShareUriWindow.h"
#include "ui_shareuriwindow.h"


#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

ShareUriWindow::ShareUriWindow(QString text, QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ShareUriWindow) {
	//Costruttore landing page
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
	centerAndResize();

	ui->lineEdit_uri->setText(text);

}


ShareUriWindow::~ShareUriWindow()
{
	delete ui;
}

void ShareUriWindow::centerAndResize() {
	//Ricava dimensione desktop
	QSize availableSize = QApplication::desktop()->availableGeometry().size();
	int width = availableSize.width();
	int height = availableSize.height();

	//Proporzionamento
	width *= 0.3;
	height *= 0.12;

	//Le dimensioni vengono fissate per rendere la finestra non resizable
	setMaximumHeight(height);
	setMinimumHeight(height);
	setMaximumWidth(width);
	setMinimumWidth(width);

	//Nuova dimensione
	QSize newSize(width, height);

	//Crea il nuovo rettangolo su cui aprire la finestra
	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, newSize, QApplication::desktop()->availableGeometry()));
}
