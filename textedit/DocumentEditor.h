#ifndef DOCUMENTEDITOR_H
#define DOCUMENTEDITOR_H

#include <QObject>
#include <Document.h>
#include <User.h>
#include "TextEdit.h"


class DocumentEditor : public QObject
{
	Q_OBJECT

private:

	Document _document;
	User& _user;
	TextEdit* _textedit;

public:

	DocumentEditor(Document doc, TextEdit* editor, User& user, QObject* parent = nullptr);
	void openDocument();

public slots:

	// Char operations
	void charsInsert(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);
	void charsDelete(QVector<Position> positions);
	void addCharsAtIndex(QVector<QChar> chars, QVector<QTextCharFormat> fmts, int pos, bool isLast, QTextBlockFormat blkFmt);
	void deleteCharsAtIndex(int position, int charCount);
	void changeSymbolFormat(int position, int count, QVector<QTextCharFormat> fmts);
	void applySymbolFormat(QVector<Position> positions, QVector<QTextCharFormat> fmts);

	void generateExtraSelection();		// Text highlighting update

	// Block operations
	void changeBlockAlignment(int start, int end, Qt::Alignment alignment);
	void changeBlockLineHeight(int start, int end, qreal height, int heightType);
	void changeBlockFormat(int start, int end, QTextBlockFormat fmt);
	void applyBlockFormat(TextBlockID blockId, QTextBlockFormat fmt);

	// List operations
	void listEditBlock(TextBlockID blockId, TextListID listId, QTextListFormat fmt);
	void createList(int position, QTextListFormat fmt);
	void assignBlockToList(int blockPosition, int listPosition);
	void removeBlockFromList(int blockPosition);
	void toggleList(int start, int end, QTextListFormat fmt);


signals:

	void charsAdded(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);
	void charsDeleted(QVector<Position> positions);
	void charsFormatChanged(QVector<Position> positions, QVector<QTextCharFormat> fmts);
	void blockFormatChanged(TextBlockID blockId, QTextBlockFormat fmt);
	void blockListChanged(TextBlockID blockId, TextListID listId, QTextListFormat fmt);
};


#endif //DOCUMENTEDITOR_H