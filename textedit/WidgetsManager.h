#ifndef WIDGETSMANAGER_H
#define WIDGETSMANAGER_H

#include <QWidget>
#include <QLabel>
#include <QString>

class WidgetsManager
{
public:
	WidgetsManager(QWidget* widget);
	~WidgetsManager();

	//Widget scaling and size
	void centerAndResize(double wScale, double hScale);

	//Loading splash screen
	void setupLoadingInfo(QLabel* _label);
	void showLoadingScreen(QLabel* _label, QString text);
	void hideLoadingScreen(QLabel* _label);
private:
	//Main widget
	QWidget* _widget;
};


#endif //WIDGETSMANAGER_H
