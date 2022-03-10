#include "WidgetsManager.h"

//#include <QDesktopWidget>
#include <QApplication>
#include <QStyle>
#include <QScreen>


WidgetsManager::WidgetsManager(QWidget* widget) : _widget(widget)
{
	//Sets widget icon
	_widget->setWindowIcon(QIcon(":/images/misc/logo.png"));
}

WidgetsManager::~WidgetsManager()
{
	//Nothing to do
}

void WidgetsManager::centerAndResize(double wScale, double hScale)
{
	//Gets desktop available size
	//QSize availableSize = QApplication::desktop()->availableGeometry().size();
	QSize availableSize = QGuiApplication::primaryScreen()->availableGeometry().size();

	int width = availableSize.width();
	int height = availableSize.height();

	//Scaling
	width *= wScale;
	height *= hScale;

	//Fixed size, not resizable window
	_widget->setMaximumHeight(height);
	_widget->setMinimumHeight(height);
	_widget->setMaximumWidth(width);
	_widget->setMinimumWidth(width);

	//Nuova dimensione
	QSize newSize(width, height);

	//Draw window in desktop center
	_widget->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, newSize,// QApplication::desktop()->availableGeometry()
		QGuiApplication::primaryScreen()->availableGeometry()));
}


void WidgetsManager::setupLoadingInfo(QLabel* _label)
{
	//Center and resize
	int labelW = _widget->width() / 2;
	int labelH = _widget->height() / 5;

	_label->resize(labelW, labelH);
	_label->move(_widget->width() / 2 - labelW / 2, _widget->height() / 2 - labelH / 2);

	//Sets white background
	_label->setAutoFillBackground(true);
	QPalette pal = _label->palette();
	pal.setColor(QPalette::Window, QColor(Qt::white));
	_label->setPalette(pal);

	//Sets frame shape of the label
	_label->setFrameShape(QFrame::WinPanel);

	//Sets word wrap to prevent clipping text
	_label->setWordWrap(true);

	//Sets font and alignment
	QFont font = _label->font();
	font.setPointSize(25);
	_label->setFont(font);

	_label->setAlignment(Qt::AlignCenter);

	//Hide label on startup
	_label->close();
}


void WidgetsManager::showLoadingScreen(QLabel* _label, QString text)
{
	//Change application cursor to WaitCursor
	QApplication::setOverrideCursor(Qt::WaitCursor);

	//Sets custom text to loading box
	_label->setText(text);
	_label->show();

	//Disables widget
	_widget->setEnabled(false);

	//To make sure  GUI events are done
	QCoreApplication::processEvents();
}

void WidgetsManager::hideLoadingScreen(QLabel* _label)
{
	//Restore default cursor
	QApplication::restoreOverrideCursor();

	//Hide loading splash screen
	_label->close();

	//Re-enables main widget
	_widget->setEnabled(true);

	//To make sure  GUI events are done
	QCoreApplication::processEvents();
}
