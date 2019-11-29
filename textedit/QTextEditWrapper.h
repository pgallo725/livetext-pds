#pragma once

#include <QObject>
#include <QSet>
#include <QTextEdit>


/* ======================== QTEXTEDIT WRAPPER ========================
*
*  QTextEdit is the core component of our LiveText editor, but the
*  set of features and supported RichText formats vastly exceeds
*  those integrated into our editor, therefore when pasting
*  contents from an external source (other applications, web pages...)
*  we need to parse the MIME data to sanitize what is being copied
*  and in some cases even prevent the pasting of such contents.
*  In practice, this is done by overriding a couple of QTextEdit methods,
*  also needed to catch resize events and propagate them to TextEdit
*/
class QTextEditWrapper : public QTextEdit 
{
	Q_OBJECT

private:

	const QSet<QString> supportedTags
	{
		"a", "/a",
		"b", "/b",
		"body", "/body",
		"br",
		"cite", "/cite",
		"dfn", "/dfn",
		"div", "/div",
		"em", "/em",
		"font", "/font",
		"h1", "/h1",
		"h2", "/h2",
		"h3", "/h3",
		"h4", "/h4",
		"h5", "/h5",
		"h6", "/h6",
		"head", "/head",
		"html", "/html",
		"i", "/i",
		"meta",
		"li", "/li",
		"ol", "/ol",
		"p", "/p",
		"qt", "/qt",
		"s", "/s",
		"span", "/span",
		"strong", "/strong",
		"sub", "/sub",
		"sup", "/sup",
		"title", "/title",
		"u", "/u",
		"ul", "/ul",
		"var", "/var"
	};

	const QSet<QString> supportedAttributes
	{
		"color",
		"font",
		"font-size",
		"font-style",
		"font-weight",
		"text-decoration"
	};

protected:

	bool canInsertFromMimeData(const QMimeData* source) const override;
	virtual void insertFromMimeData(const QMimeData* source) override;

	virtual void resizeEvent(QResizeEvent* e) override;

signals:

	void editorResizeEvent();
};