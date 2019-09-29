#include "DocumentEditor.h"
#include <Symbol.h>

DocumentEditor::DocumentEditor(Document doc, TextEdit* editor, User user, QObject* parent) : QObject(parent), _document(doc), _textedit(editor), _user(user)
{
	_textedit->setDocumentURI(doc.getURI().toString());

	//CONNECT
	connect(_textedit, &TextEdit::charDeleted, this, &DocumentEditor::deleteCharAtIndex);
	connect(_textedit, &TextEdit::charInserted, this, &DocumentEditor::addCharAtIndex);
	connect(_textedit, &TextEdit::generateExtraSelection, this, &DocumentEditor::generateExtraSelection);
	connect(_textedit, &TextEdit::blockFormatChanged, this, &DocumentEditor::blockFormatChanged);

}

void DocumentEditor::openDocument()
{
	QVector<Symbol> document = _document.getContent();

	for (int i = 0; i < document.length() - 1; i++){
		_textedit->newChar(document[i].getChar(), document[i].getFormat(), i);
	}
	_textedit->setCurrentFileName(_document.getName());
	_textedit->startCursorTimer();

	generateExtraSelection();
}


//From Server to Client
void DocumentEditor::addSymbol(Symbol s)
{
	int position = _document.insert(s);
	_textedit->newChar(s.getChar(), s.getFormat(), position, s.getAuthorId());
}

void DocumentEditor::removeSymbol(QVector<int> position)
{
	int pos = _document.removeAt(position);
	_textedit->removeChar(pos);
}

//From Client to Server
void DocumentEditor::deleteCharAtIndex(int position)
{
	QVector<qint32> fractionalPosition = _document.removeAtIndex(position);
	emit deleteChar(fractionalPosition);
}

void DocumentEditor::addCharAtIndex(QChar ch, QTextCharFormat fmt, int position)
{
	Symbol s;
	if (position == 0) {
		s = Symbol(ch, fmt, _user.getUserId(), _document.fractionalPosBegin());
	}
	else if (position == _document.length()) {
		s = Symbol(ch, fmt, _user.getUserId(), _document.fractionalPosEnd());
	}
	else {
		s = Symbol(ch, fmt, _user.getUserId(), _document.fractionalPosAtIndex(position));
	}


	_document.insert(s);
	emit insertChar(s);
}


//Generating extra selections for user
void DocumentEditor::generateExtraSelection()
{
	QPair<int, int> selectionDelimiters;
	QVector<Symbol> document = _document.getContent();

	qint32 userId = document.first().getAuthorId();
	selectionDelimiters.first = 0;
	selectionDelimiters.second = 0;

	for (int i = 0; i < document.length(); i++) {
		if (document[i].getAuthorId() != userId) {
			_textedit->setExtraSelections(userId, selectionDelimiters);

			userId = document[i].getAuthorId();
			selectionDelimiters.first = i;
			selectionDelimiters.second = i;
		}
		selectionDelimiters.second++;
	}
	_textedit->setExtraSelections(userId, selectionDelimiters);
}

//Block format
void DocumentEditor::blockFormatChanged(qint32 userId, int position, QTextBlockFormat fmt)
{
	QPair<int, int> blockId = _document.getBlockAt(position);
	_document.formatBlock(blockId, fmt);

	emit userBlockFormatChanged(blockId, fmt, userId);
}

void DocumentEditor::applyBlockFormat(QPair<int, int> blockId, QTextBlockFormat fmt, qint32 userId)
{
	int position = _document.formatBlock(blockId, fmt);
	_textedit->applyBlockFormat(userId, position, fmt);
}