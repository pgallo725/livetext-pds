#ifndef WIDGETSMANAGER_H
#define WIDGETSMANAGER_H

#include <QWidget>
#include <QLabel>

class WidgetsManager
{
private:
	//Main widget
	QWidget* _widget;

public:
	WidgetsManager(QWidget* widget);
	~WidgetsManager();

	//Widget scaling and size
	void centerAndResize(double wScale, double hScale);

	//Loading splash screen
	void setupLoadingInfo(QLabel* _label);
	void showLoadingScreen(QLabel* _label, QString text);
	void hideLoadingScreen(QLabel* _label);
};


#endif //WIDGETSMANAGER_H
