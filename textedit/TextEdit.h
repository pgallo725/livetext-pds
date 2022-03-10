#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>

#include <User.h>

#include "Presence.h"
#include "QTextEditWrapper.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB


#define CURSOR_SEND_INTERVAL 200
#define STATUSBAR_MSG_DURATION 5000
#define BULK_EDIT_SIZE 256
#define LIST_STYLES 9


//Pre-declaration of some QtWidget classes
class QMenu;
class QAction;
class QLabel;
class QComboBox;
class QFontComboBox;
class QTextCharFormat;
class QPrinter;
class QScrollArea;
class QToolButton;


//Main editor class
class TextEdit : public QMainWindow
{
	Q_OBJECT

private:

	//Document infos
	QString URI;
	QString fileName;

	//Text editor
	QTextEditWrapper* _textEdit;


	//Logged user
	User& _user;

	//Online users
	QMap<qint32, Presence> onlineUsers;
	QToolBar* onlineUsersToolbar;

	//User text highlighting
	QAction* actionHighlightUsers;
	QList<QTextEdit::ExtraSelection> _usersText;

	//Current cursor position
	int _currentCursorPosition;
	//Extra cursor to apply formats or insert/delete characters
	QTextCursor* _extraCursor;

	//Timer event
	QTimer _cursorTimer;
	QTimer _statusBarTimer;


	//Button and actions of list menu
	QToolButton* listButton;
	QAction* listActions[LIST_STYLES];

	//GUI update and list style easy application
	enum listType
	{
		standard,
		disc,
		circle,
		square,
		decimal,
		alpha,
		alphaupper,
		roman,
		romanupper
	};

	QString listIconPath[LIST_STYLES] =
	{
		"/editor/list.png",
		"/editor/disc.png",
		"/editor/circle.png",
		"/editor/square.png",
		"/editor/decimal.png",
		"/editor/alpha.png",
		"/editor/alphaupper.png",
		"/editor/roman.png",
		"/editor/romanupper.png"
	};

	QTextListFormat::Style listStyles[LIST_STYLES] =
	{
		QTextListFormat::ListStyleUndefined,
		QTextListFormat::ListDisc,
		QTextListFormat::ListCircle,
		QTextListFormat::ListSquare,
		QTextListFormat::ListDecimal,
		QTextListFormat::ListLowerAlpha,
		QTextListFormat::ListUpperAlpha,
		QTextListFormat::ListLowerRoman,
		QTextListFormat::ListUpperRoman
	};

	//Line height
	QToolButton* lineHeightButton;
	QAction* actionLineHeight100;
	QAction* actionLineHeight115;
	QAction* actionLineHeight150;
	QAction* actionLineHeight200;

	//Text format actions
	QAction* actionTextBold;
	QAction* actionTextUnderline;
	QAction* actionTextItalic;
	QAction* actionTextStrikeout;
	QAction* actionTextSubscript;
	QAction* actionTextSuperscript;
	QAction* actionTextColor;
	QAction* actionAlignLeft;
	QAction* actionAlignCenter;
	QAction* actionAlignRight;
	QAction* actionAlignJustify;
	QFontComboBox* comboFont;
	QComboBox* comboSize;

	//Copy/Cut/Paste
#ifndef QT_NO_CLIPBOARD
	QAction* actionCut;
	QAction* actionCopy;
	QAction* actionPaste;
#endif
	QAction* actionDelete;

	//Editor context menu
	QMenu* _contextMenu;
	
	//Scroll area widget to move document inside QMainWindow
	QScrollArea* area;

	
	
	/* ----------------------- METHODS ----------------------- */

	//GUI Setup
	void setupMainWindow();
	void setupEditorActions();

	//Warning message before closing document
	void askBeforeCloseDocument();

	//Apply format on selection (if present)
	void mergeFormatOnSelection(const QTextCharFormat& format);

	//Update GUI according to text format/list/alignment/line height
	void fontChanged(const QFont& f);
	void colorChanged(const QColor& c);
	void alignmentChanged(Qt::Alignment a);
	void scriptChanged(QTextCharFormat::VerticalAlignment a);
	void lineHeightChanged(qreal height);
	void toggleCheckList(int listType);

	//GUI update
	void updateEditorSelectedActions();

	//User text highlighting
	void handleMultipleSelections();
	int areUserIconActive();

	//Cursor timer
	void cursorTimerEvent();

	//Graphic cursors
	void redrawAllCursors();
	void drawGraphicCursor(const Presence& p);


public:
	TextEdit(User& user, QWidget* parent = 0);
	~TextEdit();

	//Start timer
	void startTimers();

	//Document
	void setCurrentFileName(QString fileName, QString uri);

	//REMOTE: character insertion/deletion
	void newChars(QString chars, QTextCharFormat fmt, int position);
	void removeChars(int start, int end);

	//REMOTE: Apply Symbol format
	void applyCharFormat(int position, int end, QTextCharFormat fmt);

	//REMOTE: Apply text block format
	void applyBlockFormat(int position, QTextBlockFormat fmt);

	//List management
	void createList(int position, QTextListFormat fmt);
	void removeBlockFromList(int blockPosition);
	void addBlockToList(int blockPosition, int listPosition);

	//Editor operations
	void closeEditor();
	void resetCursorPosition();

	//User text highlighting
	void setExtraSelections(qint32 userId, QPair<int, int> selection);

	/* ----------------------- SIGNALS ----------------------- */
signals:
	//Ask to close current document
	void closeDocument(int userId);

	//Send current cursor position
	void newCursorPosition(int userId, qint32 position);

	//Ask to open edit profile window
	void openEditProfile(bool inEditor = true);

	//LOCAL: character insertion/deletion/format change
	void charsAdded(QVector<QChar> chars, QVector<QTextCharFormat> fmts, int pos, bool isLast, QTextBlockFormat blkFmt);
	void charsDeleted(int position, int count);
	void charsFormatChanged(int position, int count, QVector<QTextCharFormat> fmts);

	//LOCAL: text block format changed
	void blockFormatChanged(int start, int end, Qt::Alignment alignment);
	void blockFormatChanged(int start, int end, qreal height, int heightType);
	void blockFormatChanged(int start, int end, QTextBlockFormat fmt);

	//Users text highlighting
	void generateExtraSelection();

	//List operations
	void toggleList(int start, int end, QTextListFormat fmt);
	void createNewList(int position, QTextListFormat fmt);
	void assignBlockToList(int blockPosition, int listPosition);
	void setBlockNoList(int blockPosition);


	/* ----------------------- SLOTS ----------------------- */
public slots:

	//User cursor position changed
	void userCursorPositionChanged(qint32 position, qint32 user);

	//Add/Remove users from editor
	void newPresence(qint32 userId, QString username, QImage image);
	void removePresence(qint32 userId);

	//Shows error in case of failure during document cl
	void closeDocumentError(QString error);
	void doCloseDocument(int reply);

	//Status bar messages
	void showStatusBarMessage(const QString& text);

private slots:

	//File printing
	void filePrint();
	void printDocument(QPrinter* p);
	void filePrintPreview();
	void exportPdf();
	void filePrintPdf(const QString& file);

	//Share URI
	void fileShare();

	//User text highlighting
	void highlightUsersText();

	//Intercept document modifications (character insertion/deletion)
	void contentsChange(int position, int charsRemoved, int charsAdded);

	//Text format
	void textBold();
	void textUnderline();
	void textItalic();
	void textStrikeout();
	void textSubscript();
	void textSuperscript();
	void textFamily(const QString& f);
	void textSize(const QString& p);
	void incrementSize();
	void decrementSize();
	void selectColor();
	void textColor(const QColor& color);
	void textAlign(QAction* a);

	//Lists
	void listStyle(int styleIndex);

	//LineHeight
	void setLineHeight(QAction* a);

	//GUI update according to format and cursor position
	void currentCharFormatChanged(const QTextCharFormat& format);
	void cursorPositionChanged();

	//Clipboard filter
	void clipboardDataChanged();

	//Widget resize
	void resizeEditor(const QSizeF& newSize);

	//Document custom menu
	void showCustomContextMenu(const QPoint& position);

	//About
	void showAboutWindow();
	void linkPressed();
};

#endif // TEXTEDIT_H
