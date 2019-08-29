#include "DocumentEditor.h"

DocumentEditor::DocumentEditor(Document doc, TextEdit* editor, QObject* parent) : QObject(parent), _document(doc), _textedit(editor)
{
}

void DocumentEditor::openDocument()
{

}
