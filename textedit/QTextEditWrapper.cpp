#include "QTextEditWrapper.h"

#include <QRegularExpression>
#include <QMimeData>


//Disable drag&drop of images and URLs
bool QTextEditWrapper::canInsertFromMimeData(const QMimeData* source) const 
{
	return !(source->hasImage() || source->hasUrls());
}


//Prevent pasting images overload of QTextEdit function
void QTextEditWrapper::insertFromMimeData(const QMimeData* source) 
{
	if (!source->hasImage())
	{
		if (source->hasHtml())
		{
			QString data = source->data("text/html");
			
			QRegularExpression tagsRegex("<([\/]?\\w+)([^>]*?)( \/)?>", QRegularExpression::CaseInsensitiveOption);
			QRegularExpression attributesRegex(" ([\\w-]+)=\"([^>\"]*)\"", QRegularExpression::CaseInsensitiveOption);
			QRegularExpression styleRegex("[^>]*?style=\"([^>\"]*)\"[^>]*", QRegularExpression::CaseInsensitiveOption);
			QRegularExpression propertiesRegex("([-\\w\\s]+?):([^;]+);?", QRegularExpression::CaseInsensitiveOption);
			QRegularExpression headingsRegex("<(h[1-6])([^>]*)>(.*?)<\/\\1>", QRegularExpression::CaseInsensitiveOption);

			tagsRegex.optimize();
			attributesRegex.optimize();
			styleRegex.optimize();
			propertiesRegex.optimize();
			headingsRegex.optimize();


			// Link tags are removed, the hypertext is colored blue and underlined instead
			data.replace(QRegularExpression("<a[^>]+>(.*?)<\/a>", QRegularExpression::CaseInsensitiveOption),
				"<span style=\"color:blue;text-decoration:underline;\">\\1</span>");

			// Replace some unsupported tags with paragraph blocks or spans to maintain similar formatting
			data.replace(QRegularExpression("<(table|blockquote|address|dl|pre)([^>]*?)>(.*?)<\/\\1>",
				QRegularExpression::CaseInsensitiveOption),	"<p\\2>\\3</p>");
			data.replace(QRegularExpression("<(\/?)div([^>]*?)>", QRegularExpression::CaseInsensitiveOption), "<\\1span\\2>");
			data.replace(QRegularExpression("<\/(d[dt]|[th]r)>", QRegularExpression::CaseInsensitiveOption), "<br />");
			
			
			// PARSE ALL HTML TAGS
			int delta = 0;
			QRegularExpressionMatchIterator tags = tagsRegex.globalMatch(data);
			while (tags.hasNext())
			{
				QRegularExpressionMatch match = tags.next();
				QString tag = match.captured(1);
				QString attributes = match.captured(2);

				if (!supportedTags.contains(tag))
				{
					// Remove the unsupported tag from the MIME data
					data.remove(match.capturedStart() + delta, match.capturedLength());
					delta -= match.capturedLength();
					continue;
				}

				// Some applications make use of the HTML special characters inside tag attributes, 
				// which have to be removed before the actual parsing (unnecessary anyways)
				attributes.remove("&quot;");
				attributes.remove("&amp;");


				// PARSE ALL TAG ATTRIBUTES
				QRegularExpressionMatchIterator attrs = attributesRegex.globalMatch(attributes);
				int attr_delta = 0;
				while (attrs.hasNext())
				{
					QRegularExpressionMatch match = attrs.next();
					QString attribute = match.captured(1);

					// Remove all unsupported tag attributes
					if (!supportedAttributes.contains(attribute))
					{
						attributes.remove(match.capturedStart() + attr_delta, match.capturedLength());
						attr_delta -= match.capturedLength();
					}
				}

				if (attributes.isEmpty() || !attributes.contains("style="))
				{
					if (!tag.contains('/') && !unstyledTags.contains(tag))
					{
						// Force-add a 0px margin style attribute to all opening tags
						attributes.append(" style=\"margin: 0px;\"");
					}
				}
				else
				{
					QRegularExpressionMatch styleMatch = styleRegex.match(attributes);
					QString style = styleMatch.captured(1);

					// PARSE ALL STYLE PROPERTIES ONE BY ONE
					int style_delta = 0;
					QRegularExpressionMatchIterator props = propertiesRegex.globalMatch(style);
					while (props.hasNext())
					{
						QRegularExpressionMatch propMatch = props.next();
						QString property = propMatch.captured(1).trimmed();
						QString value = propMatch.captured(2);

						if (!supportedProperties.contains(property))
						{
							// Remove the unsupported value from the style properties
							style.remove(propMatch.capturedStart() + style_delta, propMatch.capturedLength());
							style_delta -= propMatch.capturedLength();
						}
						else
						{
							// Sanitize font property fields
							if (property == "font")
							{
								QString font = QChar::Space + correctFont(value);
								style.replace(propMatch.capturedStart(2) + style_delta, propMatch.capturedLength(2), font);
								style_delta += font.length() - propMatch.capturedLength(2);
							}
							// Replace font-size properties with actual sizes in pt
							if (property == "font-size")
							{
								QString fontSize = QChar::Space + correctFontSize(value.trimmed());
								style.replace(propMatch.capturedStart(2) + style_delta, propMatch.capturedLength(2), fontSize);
								style_delta += fontSize.length() - propMatch.capturedLength(2);
							}
							// Replace different font weight types with just normal and bold
							else if (property == "font-weight")
							{
								if (!value.contains(QRegularExpression("normal|bold", QRegularExpression::CaseInsensitiveOption)))
								{
									QString fontWeight = QChar::Space + correctFontWeight(value);
									style.replace(propMatch.capturedStart(2) + style_delta, propMatch.capturedLength(2), fontWeight);
									style_delta += fontWeight.length() - propMatch.capturedLength(2);
								}
							}
							// Only subscript or superscript alignments are allowed in the document
							else if (property == "vertical-align")
							{
								if (!value.contains(QRegularExpression("sub|super", QRegularExpression::CaseInsensitiveOption)))
								{
									// Remove the unsupported value from the style properties
									style.remove(propMatch.capturedStart() + style_delta, propMatch.capturedLength());
									style_delta -= propMatch.capturedLength();
								}
							}
							// Translate line height values into a supported size
							else if (property == "line-height")
							{
								QString lineHeight = QChar::Space + correctLineHeight(value);
								style.replace(propMatch.capturedStart(2) + style_delta, propMatch.capturedLength(2), lineHeight);
								style_delta += lineHeight.length() - propMatch.capturedLength(2);
							}
						}
					}

					// Add a 0px margin to the style attributes
					style.append("margin: 0px;");
					
					attributes.replace(styleMatch.capturedStart(1), styleMatch.capturedLength(1), style.trimmed());
				}

				// Replace the original attributes string with the sanitized one
				data.replace(match.capturedStart(2) + delta, match.capturedLength(2), attributes);
				delta += attributes.length() - match.capturedLength(2);
			}


			// Headings are replaced with normal paragraphs using bold font with the proper size
			delta = 0;
			QRegularExpressionMatchIterator it = headingsRegex.globalMatch(data);
			while (it.hasNext())
			{
				QRegularExpressionMatch match = it.next();
				QString heading = match.captured(1);
				QString attributes = match.captured(2);
				QString fontSize;

				if (heading.compare("h1", Qt::CaseInsensitive) == 0)		fontSize = "24";	// H1 -> 24pt (bold)
				else if (heading.compare("h2", Qt::CaseInsensitive) == 0)	fontSize = "18";	// H2 -> 18pt (bold)
				else if (heading.compare("h3", Qt::CaseInsensitive) == 0)	fontSize = "14";	// H3 -> 14pt (bold)
				else if (heading.compare("h4", Qt::CaseInsensitive) == 0)	fontSize = "12";	// H4 -> 12pt (bold)
				else if (heading.compare("h5", Qt::CaseInsensitive) == 0)	fontSize = "10";	// H5 -> 10pt (bold)
				else if (heading.compare("h6", Qt::CaseInsensitive) == 0)	fontSize = "8";		// H6 -> 8pt (bold)

				// After the previous parsing step, all tags have a style attribute
				int pos = attributes.indexOf("style=\"");

				// Add the new style settings to the existing attributes (if they're not already specified)
				if (!attributes.contains("font-weight", Qt::CaseInsensitive))
					attributes.insert(pos + 7, "font-weight: bold;");

				if (!attributes.contains("font-size", Qt::CaseInsensitive))
					attributes.insert(pos + 7, QString("font-size: ") + fontSize + "pt;");


				// Replace the heading tag with an equivalent paragraph tag preceded by an empty line
				QString str = QString("<p") + attributes + ">" + match.captured(3) + "</p>";
				data.replace(match.capturedStart() + delta, match.capturedLength(), str);
				delta += str.length() - match.capturedLength();
			}

			// HTML special characters (not part of tag attributes) are replaced with the proper symbol
			data.replace("&quot;", "\"");
			data.replace("&amp;", "&");

			QMimeData sanitizedSource;
			sanitizedSource.setData("text/html", data.toUtf8());
			QTextEdit::insertFromMimeData(&sanitizedSource);
			//textCursor().insertText(data);
		}
		else
		{
			QTextEdit::insertFromMimeData(source);
		}
	}
}



/*************** PRIVATE HELPER FUNCTIONS ***************/


QString QTextEditWrapper::correctFont(QString value)
{
	QString fontStyle;
	QString fontWeight;
	QString fontSize;
	QString lineHeight;
	QString fontFamily;

	value.remove("small-caps");
	value.remove("lighter");
	value.remove("normal");

	QStringList elements = value.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

	while (!elements.isEmpty())
	{
		QString element = elements.front();

		if (element == "italic" || element == "oblique") 
		{
			fontStyle = element;
		}
		else if (element.contains("bold") || element.contains("00")) 
		{
			fontWeight = element;
		}
		else 
		{
			if (fontSize.isNull())
			{
				QStringList parts = element.split("/");
				fontSize = correctFontSize(parts[0]);
				if (parts.length() > 1)
					lineHeight = correctLineHeight(parts[1]);
			}
			else
			{
				fontFamily = elements.join(" ");
				break;
			}
		}

		//Remove the element after parsing
		elements.pop_front();
	}

	// Build up again the sanitized font property string
	QString font;
	font.append(fontStyle);
	if (!font.isEmpty())
		font.append(QChar::Space);
	font.append(fontWeight);
	if (!font.isEmpty())
		font.append(QChar::Space);
	font.append(fontSize);
	if (!lineHeight.isNull())
		font.append("/" + lineHeight);
	font.append(QChar::Space + fontFamily);

	return font;
}


QString QTextEditWrapper::correctFontSize(QString value)
{
	QString fontSize = value;

	if (value.contains("px", Qt::CaseInsensitive))
	{
		// Translate px -> pt
		value.remove("px", Qt::CaseInsensitive);
		int px = value.toInt();
		int pt = std::ceil(px * 0.75f);				// 1px = 0.75pt
		fontSize = QString::number(pt) + "pt";
	}
	else if (value.contains("em", Qt::CaseInsensitive))
	{
		// Translate em/rem -> pt
		value.remove("r", Qt::CaseInsensitive);
		value.remove("em", Qt::CaseInsensitive);
		float em = value.toFloat();
		int pt = std::ceil(em * 12);				// 1em = 12pt
		fontSize = QString::number(pt) + "pt";
	}
	else if (value.contains("%"))
	{
		// Translate % -> pt
		value.remove("%", Qt::CaseInsensitive);
		float em = value.toFloat() / 100.0f;
		int pt = std::ceil(em * 12);				// 100% = 1em = 12pt
		fontSize = QString::number(pt) + "pt";
	}
	else if (!value.contains("pt", Qt::CaseInsensitive))
	{
		if (value.compare("small", Qt::CaseInsensitive) == 0)			fontSize = "10pt";	 // small	 ->  10pt 
		else if (value.compare("medium", Qt::CaseInsensitive) == 0)		fontSize = "12pt";	 // medium	 ->  12pt
		else if (value.compare("large", Qt::CaseInsensitive) == 0)		fontSize = "14pt";	 // large	 ->  14pt
		else if (value.compare("x-large", Qt::CaseInsensitive) == 0)	fontSize = "18pt";	 // x-large	 ->  18pt
		else if (value.compare("xx-large", Qt::CaseInsensitive) == 0)	fontSize = "24pt";	 // xx-large ->  24pt
	}

	return fontSize;
}


QString QTextEditWrapper::correctFontWeight(QString value)
{
	// Mapping between numeric values and types
	int n = value.toInt();
	return (n < 500 ? "normal" : "bold");		// < 500 = Normal, > 500 = Bold
}


QString QTextEditWrapper::correctLineHeight(QString value)
{
	int lineHeight = 100;

	if (value.contains("%"))
	{
		value.remove("%", Qt::CaseInsensitive);
		lineHeight = value.toInt();
	}
	else
	{
		// Verify that the line height is specified as a number, NOT in pt, px, cm...
		QRegularExpression re("^[\\s\\d\\.,]+$", QRegularExpression::CaseInsensitiveOption);
		if (re.match(value).hasMatch())
		{
			// Translate number -> %
			float n = value.toFloat();
			lineHeight = std::floor(n * 100.0f);
		}
	}

	// Normalize the % to the closest of the 4 supported values (100, 115, 150, 200)
	if (lineHeight < 105) lineHeight = 100;
	else if (lineHeight < 130) lineHeight = 115;
	else if (lineHeight < 175) lineHeight = 150;
	else lineHeight = 200;

	return QString::number(lineHeight) + "%";
}
