#include "OpenUriWindow.h"
#include "ui_openuriwindow.h"


#include <QWidget>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>

OpenUriWindow::OpenUriWindow(LandingPage* lp, QWidget* parent) : landingPage(lp), QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint), ui(new Ui::OpenUriWindow) {
	//Costruttore landing page
	setWindowTitle(QCoreApplication::applicationName());
	setWindowIcon(QIcon(":/images/logo.png"));

	//Setup delle varie finestre ui
	ui->setupUi(this);
	centerAndResize();
	
	//Imposto la label "Incorrect URI" non visibile
	ui->label_incorrectUri->setVisible(false);

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &OpenUriWindow::acceptClicked);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &OpenUriWindow::rejectClicked);
}

OpenUriWindow::~OpenUriWindow()
{
	delete ui;
}

void OpenUriWindow::acceptClicked()
{
	QString uri = ui->lineEdit_uri->text();
	bool notvalid = false;

	if (uri == "" || notvalid) {
		ui->label_incorrectUri->setVisible(true);
	}
	else {
		this->close();
		landingPage->openEditor(uri);	
	}
}

void OpenUriWindow::rejectClicked()
{
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
