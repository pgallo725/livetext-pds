#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>


#include <User.h>

#include "Presence.h"
#include "ShareUriWindow.h"

#define CURSOR_SEND_INTERVAL 250
#define STATUSBAR_MSG_DURATION 5000

//Method encapsulation
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QScrollArea;
class QTextCharFormat;
class QMenu;
class QPrinter;
class QLabel;
class QToolButton;



//This class is necessary to prevent pasting / drag & drop images inside editors
class QTextEditWrapper : public QTextEdit {

protected:
	virtual void insertFromMimeData(const QMimeData* source);
	bool canInsertFromMimeData(const QMimeData* source) const;
};



//Main editor class
class TextEdit : public QMainWindow
{
	Q_OBJECT

private:
	//Logged user
	User& _user;

	//Online users
	QMap<qint32, Presence*> onlineUsers;
	QToolBar* onlineUsersToolbar;
	//User text highlighting
	QAction* actionHighlightUsers;

	//Button and actions of list menu
	QToolButton* listButton;
	QAction* listActions[9];

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

	QString listIconPath[9] =
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

	QTextListFormat::Style listStyles[9] =
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

	//Current cursor position
	int _currentCursorPosition;
	//Extra cursor to apply formats or insert/delete characters
	QTextCursor* _extraCursor;

	//Document infos
	QString URI;
	QString fileName;

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

	//Text editor
	QTextEditWrapper* _textEdit;

	//Scroll area widget to move document inside QMainWindow
	QScrollArea* area;

	//Timer event
	QTimer _cursorTimer;

	//Share URI window
	ShareUriWindow* _shareUri;


	/* ----------------------- METHODS ----------------------- */

	//GUI Setup
	void setupMainWindow();
	void setupEditorActions();
	void setupOnlineUsersActions();

	//Warning message before closing document
	void askBeforeCloseDocument();

	//Apply format on selection (if present)
	void mergeFormatOnSelection(const QTextCharFormat& format);

	//Update GUI according to text format/list/alignment/line height
	void fontChanged(const QFont& f);
	void colorChanged(const QColor& c);
	void alignmentChanged(Qt::Alignment a);
	void lineHeightChanged(qreal height);
	void toggleCheckList(int listType);

	//GUI update
	void updateEditorSelectedActions();

	//User text highlighting
	void handleMultipleSelections();

	//Graphic cursors
	void redrawAllCursors();
	void drawGraphicCursor(Presence* p);

	//Status bar messages
	void showStatusBarMessage(QString text);

protected:
	void cursorTimerEvent();

public:
	TextEdit(User& user, QWidget* parent = 0);

	//Start timer
	void startCursorTimer();

	//Document
	void setCurrentFileName(QString fileName, QString uri);

	//REMOTE: character insertion/deletion
	void newChar(QChar ch, QTextCharFormat format, int position);
	void removeChar(int position);

	//REMOTE: Apply text block format
	void applyBlockFormat(int position, QTextBlockFormat fmt);

	//REMOTE: Apply Symbol format
	void applyCharFormat(int position, QTextCharFormat fmt);

	//List management
	void createList(int position, QTextListFormat fmt);
	void removeBlockFromList(int blockPosition);
	void addBlockToList(int blockPosition, int listPosition);

	//Editor operations (close/error)
	void closeEditor();
	void criticalError(QString error);
	void resetCursorPosition();

	//User text highlighting
	void updateUsersSelections();
	void setExtraSelections(qint32 userId, QPair<int, int> selection);

	/* ----------------------- SIGNALS ----------------------- */
signals:
	//Ask to close current document
	void closeDocument(int userId);

	//Send current cursor position
	void newCursorPosition(int userId, qint32 position);

	//Ask to open edit profile window
	void openEditProfile();

	//LOCAL: character insertion/deletion
	void charInserted(QChar ch, QTextCharFormat format, int position, bool isLast);
	void charDeleted(int position);

	//LOCAL: text block format changed
	void blockFormatChanged(int start, int end, Qt::Alignment alignment);
	void blockFormatChanged(int start, int end, qreal height, int heightType);
	void blockFormatChanged(int start, int end, QTextBlockFormat fmt);

	//LOCAL: character format changed
	void symbolFormatChanged(int position, QTextCharFormat fmt);

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

	//Shows error in case of failure during document closing
	void closeDocumentError(QString error);


	//Bindend to actions
private slots:
	//File printing
	void filePrint();
	void filePrintPreview();
	void filePrintPdf();
	void printPreview(QPrinter*);

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
	void textFamily(const QString& f);
	void textSize(const QString& p);
	void incrementSize();
	void decrementSize();
	void textColor();
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
};

#endif // TEXTEDIT_H
