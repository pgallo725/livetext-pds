#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QPointer>
#include <QMap>
#include <User.h>
#include "Presence.h"
#include "ProfileEditWindow.h"

#define CURSOR_SEND_INTERVAL 100

//Serve per incapsulare tutti i metodi delle classi
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;
class QLabel;
class QToolButton;


class TextEdit : public QMainWindow
{
	Q_OBJECT

public:
	TextEdit(QWidget* parent = 0);

	void setUser(User* user); //User logged


	//Document
	void startCursorTimer();
	void setCurrentFileName(QString fileName, QString uri);

	void newChar(QChar ch, QTextCharFormat format, int position, qint32 user = -1);
	void removeChar(int position);

	void closeEditor();

	void setExtraSelections(qint32 userId, QPair<int, int> selection);
	
	//Block format
	void applyBlockFormat(int position, QTextBlockFormat fmt);

	//Symbol format
	void applyCharFormat(int position, QTextCharFormat fmt);

	//List management
	void createList(int position, QTextListFormat fmt);
	void removeBlockFromList(int blockPosition);
	void addBlockToList(int listPosition, int blockPosition);

	void criticalError(QString error);
	void resetUndoRedo();

	//Extra selections
	void updateUsersSelections();

public slots:
	void userCursorPositionChanged(qint32 position, qint32 user);
	void newPresence(qint32 userId, QString username, QImage image);
	void removePresence(qint32 userId);
	void closeDocumentError(QString error);

signals:
	void closeDocument();
	void newCursorPosition(qint32 position);
	void openEditProfile();
	void accountUpdate(QString nickname, QImage image, QString password);
	void charDeleted(int position);
	void charInserted(QChar ch, QTextCharFormat format, int position, bool isLast);
	void generateExtraSelection();
	void blockFormatChanged(int start, int end, QTextBlockFormat fmt);
	void symbolFormatChanged(int position, QTextCharFormat fmt);

	void toggleList(int start, int end, QTextListFormat fmt);
	void createNewList(int position, QTextListFormat fmt);
	void assignBlockToList(int blockPosition, int listPosition);

private slots:
	void filePrint();
	void filePrintPreview();
	void filePrintPdf();

	void fileShare();

	void highlightUsersText();
	void contentsChange(int position, int charsRemoved, int charsAdded);

	void textBold();
	void textUnderline();
	void textItalic();
	void textFamily(const QString& f);
	void textSize(const QString& p);
	void listStyle(int styleIndex);
	void textColor();
	void textAlign(QAction* a);

	void currentCharFormatChanged(const QTextCharFormat& format);
	void cursorPositionChanged();

	void clipboardDataChanged();
	void printPreview(QPrinter*);


private:
	void setupFileActions();
	void setupEditActions();
	void setupTextActions();
	void setupShareActions();
	void setupUserActions();
	void setupOnlineUsersActions();

	void askBeforeCloseDocument();

	void mergeFormatOnSelection(const QTextCharFormat& format);
	void fontChanged(const QFont& f);
	void colorChanged(const QColor& c);
	void alignmentChanged(Qt::Alignment a);

	//Extra selections
	void handleMultipleSelections();


	//Graphic cursors
	void redrawAllCursors();
	void drawGraphicCursor(Presence* p);

	//DEBUG
	void printDocumenText();


	//List type
	QToolButton* listButton;
	enum listType { standard, disc, circle, square, decimal, alpha, alphaupper, roman, romanupper };

	//Users
	QMap<qint32, Presence*> onlineUsers;
	
	//Current cursor position
	int _currentCursorPosition;
	QTextCursor* _extraCursor;

	//Logged user
	User* _user;

	//Document
	QString URI;

	QAction* actionTextBold;
	QAction* actionTextUnderline;
	QAction* actionTextItalic;
	QAction* actionTextColor;
	QAction* actionAlignLeft;
	QAction* actionAlignCenter;
	QAction* actionAlignRight;
	QAction* actionAlignJustify;
	QAction* actionUndo;
	QAction* actionRedo;
	QAction* actionHighlightUsers;

	//Lists actions
	QAction* listStandard;
	QAction* listDisc;
	QAction* listCircle;
	QAction* listSquare;
	QAction* listDecimal;
	QAction* listAlpha;
	QAction* listAlphaUpper;
	QAction* listRoman;
	QAction* listRomanUpper;

#ifndef QT_NO_CLIPBOARD
	QAction* actionCut;
	QAction* actionCopy;
	QAction* actionPaste;
#endif

	QFontComboBox* comboFont;
	QComboBox* comboSize;

	QToolBar* onlineUsersToolbar;
	QString fileName;
	QTextEdit* textEdit;
	int timerId;

protected:
	void timerEvent(QTimerEvent* event);

};

#endif // TEXTEDIT_H
