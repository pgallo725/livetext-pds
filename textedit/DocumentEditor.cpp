#include "DocumentEditor.h"
#include <Symbol.h>

DocumentEditor::DocumentEditor(Document doc, TextEdit* editor, User user, QObject* parent) : QObject(parent), _document(doc), _textedit(editor), _user(user)
{
	_textedit->setDocumentURI(doc.getURI().toString());

	//CONNECT
	connect(_textedit, &TextEdit::deleteChar, this, &DocumentEditor::deleteCharAtIndex);
	connect(_textedit, &TextEdit::insertChar, this, &DocumentEditor::addCharAtIndex);

}

void DocumentEditor::openDocument()
{
	_textedit->loadDocument(_document.toString()); //TEST
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


void DocumentEditor::deleteCharAtIndex(int position)
{
	QVector<qint32> fractionalPosition = _document.removeAtIndex(position);
	emit deleteChar(fractionalPosition);
}