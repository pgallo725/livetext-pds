#include "OpenUriWindow.h"
#include "ui_openuriwindow.h"

#include "LandingPage.h"

#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

OpenUriWindow::OpenUriWindow(LandingPage* lp, QWidget* parent) : landingPage(lp), QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::OpenUriWindow) {
	//Costruttore landing page
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
	centerAndResize();
	
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &OpenUriWindow::acceptClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &OpenUriWindow::rejectClicked);
}

OpenUriWindow::~OpenUriWindow()
{
	delete ui;
}

void OpenUriWindow::incorrectOperation(QString error)
{
	ui->label_incorrectUri->setText(error);
}

void OpenUriWindow::acceptClicked()
{
	QString uri = ui->lineEdit_uri->text();

	if (uri.isEmpty()) {
		incorrectOperation(tr("Please insert a valid URI"));
		return;
	}

	emit landingPage->addDocument(uri);
}

void OpenUriWindow::rejectClicked()
{
	ui->label_incorrectUri->setText("");
	ui->lineEdit_uri->setText("");
	this->close();
}

void OpenUriWindow::centerAndResize() {
	//Ricava dimensione desktop
	QSize availableSize = QApplication::desktop()->availableGeometry().size();
	int width = availableSize.width();
	int height = availableSize.height();

	//Proporzionamento
	width *= 0.3;
	height *= 0.15;

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
