#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QPointer>


//Serve per incapsulare tutti i metodi delle classi
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;
class QLabel;


class TextEdit : public QMainWindow
{
	Q_OBJECT

public:
	TextEdit(QWidget *parent = 0);

	bool load(const QString &f);

public slots:
	void fileNew(QString name);

protected:
	void closeEvent(QCloseEvent *e) override;

private slots:
	void fileOpen();
	bool fileSave();
	bool fileSaveAs();
	void filePrint();
	void filePrintPreview();
	void filePrintPdf();

	//ADDED
	void fileShare();
	void editProfile();
	void highlightUsersText();
	void contentsChange(int position, int charsRemoved, int charsAdded);

	void textBold();
	void textUnderline();
	void textItalic();
	void textFamily(const QString &f);
	void textSize(const QString &p);
	void textStyle(int styleIndex);
	void textColor();
	void textAlign(QAction *a);

	void currentCharFormatChanged(const QTextCharFormat &format);
	void cursorPositionChanged();


	void clipboardDataChanged();
	void about();
	void printPreview(QPrinter *);

	
private:
	void setupFileActions();
	void setupEditActions();
	void setupTextActions();
	void setupShareActions();
	void setupUserActions();
	bool maybeSave();
	void setCurrentFileName(const QString &fileName);

	void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
	void fontChanged(const QFont &f);
	void colorChanged(const QColor &c);
	void alignmentChanged(Qt::Alignment a);

	void handleLabel();
	void handleMultipleSelections();
	QLabel *guestCursor;
	QLabel* guestCursor2;

	QAction *actionSave;
	QAction *actionTextBold;
	QAction *actionTextUnderline;
	QAction *actionTextItalic;
	QAction *actionTextColor;
	QAction *actionAlignLeft;
	QAction *actionAlignCenter;
	QAction *actionAlignRight;
	QAction *actionAlignJustify;
	QAction *actionUndo;
	QAction *actionRedo;
	QAction *actionShare;
	QAction *actionUser;
	QAction* actionHighlightUsers;

#ifndef QT_NO_CLIPBOARD
	QAction *actionCut;
	QAction *actionCopy;
	QAction *actionPaste;
#endif

	QComboBox *comboStyle;
	QFontComboBox *comboFont;
	QComboBox *comboSize;

	QToolBar *tb;
	QString fileName;
	QTextEdit *textEdit;

};

#endif // TEXTEDIT_H