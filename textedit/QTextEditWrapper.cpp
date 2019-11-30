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
			QRegularExpression styleRegex("[^>]*?style=\"([^>\"]*)\"[^>]*", QRegularExpression::CaseInsensitiveOption);
			QRegularExpression attributesRegex("([-\\w\\s]+?):([^;]+);?", QRegularExpression::CaseInsensitiveOption);
			QRegularExpression headingsRegex("<(h[1-6])([^>]*)>(.*?)<\/\\1>", QRegularExpression::CaseInsensitiveOption);

			tagsRegex.optimize();
			styleRegex.optimize();
			attributesRegex.optimize();
			headingsRegex.optimize();


			// Link tags are removed, the hypertext is colored blue and underlined instead
			data.replace(QRegularExpression("<a[^>]+>(.*?)<\/a>", QRegularExpression::CaseInsensitiveOption),
				"<span style=\"color:blue;text-decoration:underline;\">\\1</span>");

			// Clean up all empty tags
			data.remove(QRegularExpression("<(\\w+)[^>]*?>\\s*(?R)*\\s*<\/\\1>", QRegularExpression::CaseInsensitiveOption));

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


				if (attributes.isEmpty() || !attributes.contains("style="))
				{
					if (!tag.contains('/') && !unstyledTags.contains(tag))
					{
						// Force-add a 0px margin style attribute to all opening tags
						attributes.append(" style=\"margin:0px;\"");
					}
				}
				else
				{
					QRegularExpressionMatch styleMatch = styleRegex.match(attributes);
					QString style = styleMatch.captured(1);

					// PARSE ALL STYLE ATTRIBUTES ONE BY ONE
					int style_delta = 0;
					QRegularExpressionMatchIterator attrs = attributesRegex.globalMatch(style);
					while (attrs.hasNext())
					{
						QRegularExpressionMatch attrMatch = attrs.next();
						QString attribute = attrMatch.captured(1).trimmed();
						QString value = attrMatch.captured(2);

						if (!supportedAttributes.contains(attribute))
						{
							// Remove the unsupported value from the style attributes
							style.remove(attrMatch.capturedStart() + style_delta, attrMatch.capturedLength());
							style_delta -= attrMatch.capturedLength();
						}
						else
						{
							// Replace font-size properties with actual sizes in pt
							if (attribute == "font-size")
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
									// Translate em -> pt
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
								
								style.replace(attrMatch.capturedStart(2) + style_delta, attrMatch.capturedLength(2), fontSize);
								style_delta += fontSize.length() - attrMatch.capturedLength(2);
							}
							// Replace different font weight types with just normal and bold
							else if (attribute == "font-weight")
							{
								if (!value.contains(QRegularExpression("normal|bold", QRegularExpression::CaseInsensitiveOption)))
								{
									// Mapping between numeric values and types
									int n = value.toInt();					
									QString fontWeight = n < 500 ? "normal" : "bold";	// < 500 = Normal, > 500 = Bold

									style.replace(attrMatch.capturedStart(2) + style_delta, attrMatch.capturedLength(2), fontWeight);
									style_delta += fontWeight.length() - attrMatch.capturedLength(2);
								}
							}
							// Only subscript or superscript alignments are allowed in the document
							else if (attribute == "vertical-align")
							{
								if (!value.contains(QRegularExpression("sub|super", QRegularExpression::CaseInsensitiveOption)))
								{
									// Remove the unsupported value from the style attributes
									style.remove(attrMatch.capturedStart() + style_delta, attrMatch.capturedLength());
									style_delta -= attrMatch.capturedLength();
								}
							}
							// Translate line height values into a supported size
							else if (attribute == "line-height")
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

								QString str = QString::number(lineHeight) + "%";
								style.replace(attrMatch.capturedStart(2) + style_delta, attrMatch.capturedLength(2), str);
								style_delta += str.length() - attrMatch.capturedLength(2);
							}
						}
					}

					// Add a 0px margin to the style attributes
					style.append("margin:0px;");
					
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
					attributes.insert(pos + 7, "font-weight:bold;");

				if (!attributes.contains("font-size", Qt::CaseInsensitive))
					attributes.insert(pos + 7, QString("font-size:") + fontSize + "pt;");


				// Replace the heading tag with an equivalent paragraph tag preceded by an empty line
				QString str = QString("<p") + attributes + ">" + match.captured(3) + "</p>";
				data.replace(match.capturedStart() + delta, match.capturedLength(), str);
				delta += str.length() - match.capturedLength();
			}


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


void QTextEditWrapper::resizeEvent(QResizeEvent* e)
{
	QTextEdit::resizeEvent(e);
	emit editorResizeEvent();
}
