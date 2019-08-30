#include "DocumentEditor.h"

DocumentEditor::DocumentEditor(Document doc, TextEdit* editor, QObject* parent) : QObject(parent), _document(doc), _textedit(editor)
{
	_textedit->setDocumentURI(doc.getURI().toString());

	//CONNECT
	connect(_textedit, &TextEdit::deleteChar, this, &DocumentEditor::deleteCharAtIndex);

}

void DocumentEditor::openDocument()
{
}


void DocumentEditor::deleteCharAtIndex(int position)
{
	QVector<qint32> fractionalPosition = _document.removeAtIndex(position);
	emit deleteChar(fractionalPosition);
}