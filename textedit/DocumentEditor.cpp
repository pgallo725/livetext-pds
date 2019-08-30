#include "DocumentEditor.h"

DocumentEditor::DocumentEditor(Document doc, TextEdit* editor, QObject* parent) : QObject(parent), _document(doc), _textedit(editor)
{
	_textedit->setDocumentURI(doc.getURI().toString());

	//CONNECT
	connect(_textedit, &TextEdit::deleteChar, this, &DocumentEditor::deleteChar);

}

void DocumentEditor::openDocument()
{

}

void DocumentEditor::deleteChar(int position)
{

}