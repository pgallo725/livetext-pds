#ifndef SHAREURIWINDOW_H
#define SHAREURIWINDOW_H

#include <QDialog>
#include <textedit.h>

namespace Ui {
	class ShareUriWindow;
}


class ShareUriWindow : public QDialog
{
	Q_OBJECT

public:
	ShareUriWindow(QString text, TextEdit* editor, QWidget* parent = nullptr);
	~ShareUriWindow();
	Ui::ShareUriWindow* ui;

private:
	void centerAndResize();
	void copyAndClose();

	QString _uri;
	TextEdit* _textEdit;

};




#endif //SHAREURIWINDOW_H