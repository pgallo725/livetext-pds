#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>


namespace Ui {
	class AboutWindow;
}

class AboutWindow : public QDialog
{
	Q_OBJECT

public:
	Ui::AboutWindow* ui;
	AboutWindow(QWidget* parent = nullptr);
	~AboutWindow();

};


#endif //ABOUTWINDOW_H
