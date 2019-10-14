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
	void setDocumentURI(QString uri);
	void startCursorTimer();
	void setCurrentFileName(const QString& fileName);

	void newChar(QChar ch, QTextCharFormat format, int position, qint32 user = -1);
	void removeChar(int position);

	void closeEditor();

	void setExtraSelections(qint32 userId, QPair<int, int> selection);
	
	//Block format
	void applyBlockFormat(int position, QTextBlockFormat fmt);

	//Symbol format
	void applyCharFormat(int position, QTextCharFormat fmt);

	void criticalError(QString error);

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
	void charInserted(QChar ch, QTextCharFormat format, int position);
	void generateExtraSelection();
	void blockFormatChanged(int start, int end, QTextBlockFormat fmt);
	void symbolFormatChanged(qint32 userId, int position, QTextCharFormat fmt);

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
	void textStyle(int styleIndex);
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

	void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
	void fontChanged(const QFont& f);
	void colorChanged(const QColor& c);
	void alignmentChanged(Qt::Alignment a);

	//Extra selections and multiple cursors
	void handleUsersCursors();
	void handleMultipleSelections();
	void updateUsersSelections();

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
	QAction* actionShare;
	QAction* actionUser;
	QAction* actionHighlightUsers;
	QAction* actioncloseDocument;

#ifndef QT_NO_CLIPBOARD
	QAction* actionCut;
	QAction* actionCopy;
	QAction* actionPaste;
#endif

	QComboBox* comboStyle;
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