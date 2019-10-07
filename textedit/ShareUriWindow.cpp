#include "ShareUriWindow.h"
#include "ui_shareuriwindow.h"


#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QClipboard>
#include <QStatusBar>

const QString rsrcPath = ":/images/win";

ShareUriWindow::ShareUriWindow(QString text, TextEdit* editor, QWidget* parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::ShareUriWindow), _textEdit(editor){
	//Costruttore landing page
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
	centerAndResize();

	ui->lineEdit_uri->setText(text);

	_uri = text;
	
	//Copy icon
	ui->pushButton_copy->setIcon(QIcon::QIcon(rsrcPath + "/editcopy.png"));

	//Connect pushbutton
	connect(ui->pushButton_copy, &QPushButton::clicked, this, &ShareUriWindow::copyAndClose);
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

void ShareUriWindow::copyAndClose()
{
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(_uri);
	this->close();
	_textEdit->statusBar()->showMessage(tr("URI copied into clipboards"));
}
