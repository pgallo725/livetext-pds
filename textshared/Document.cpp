#include "Document.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>

#include <iostream>

#define DOCUMENTS_DIRNAME "./Documents/"
#define URI_FIELD_SEPARATOR '_'


URI::URI()
{
}

URI::URI(QString uri)
	: str(uri)
{
}

QString URI::toString()
{
	return str;
}

std::string URI::toStdString()
{
	return str.toStdString();
}

QString URI::getAuthorName()
{
	// The first field of the generated URI (fields separated by '_') 
	// is the name of the user which created the document
	return str.section(URI_FIELD_SEPARATOR, 0, 0);
}

QString URI::getDocumentName()
{
	// The second field of the generated URI (fields separated by '_') 
	// is the name of the document (including .extension)
	return str.section(URI_FIELD_SEPARATOR, 1, 1);
}

bool URI::operator<(const URI& other) const noexcept
{
	return this->str < other.str;
}

bool URI::operator>(const URI& other) const noexcept
{
	return this->str > other.str;
}

bool URI::operator==(const URI& other) const noexcept
{
	return this->str == other.str;
}

QDataStream& operator>>(QDataStream& in, URI& uri)
{
	in >> uri.str;

	return in;
}

QDataStream& operator<<(QDataStream& out, const URI& uri)
{
	out << uri.str;

	return out;
}


/*************** DOCUMENT METHODS ***************/


Document::Document()
	: _blockCounter(0)
{
}

Document::Document(URI docURI) :
	uri(docURI), _blockCounter(0)
{
	editors << getAuthor();

	// Insert a ParagraphTerminator character inside a default block in the empty document
	TextBlock defaultBlock = TextBlock(_blockCounter++, -1, QTextBlockFormat());
	Symbol eof = Symbol(QChar::ParagraphSeparator, QTextCharFormat(), -1, QVector<qint32>({ 1000, 1000 }));
	eof.assignToBlock(defaultBlock);

	_blocks.insert(defaultBlock.getIdPair(), defaultBlock);
	_text.insert(_text.begin(), eof);
}

Document::~Document()
{
	//NOTHING TO DO
}


URI Document::getURI()
{
	return uri;
}

QString Document::getName()
{
	return uri.getDocumentName();
}

QString Document::getAuthor()
{
	return uri.getAuthorName();
}

QVector<Symbol> Document::getContent()
{
	return _text;
}

void Document::insertNewEditor(QString edit)
{
	if (!editors.contains(edit))
		editors << edit;
}


void Document::load()
{
	// Create or overwrite the document file on disk, and write data to it
	QFile file(DOCUMENTS_DIRNAME + uri.toString());
	if (file.open(QIODevice::ReadWrite))
	{
		QDataStream docFileStream(&file);

		std::cout << "\nLoading document \"" << uri.toStdString() << "\"... ";

		// Load the document content (_text vector<Symbol>) from file
		// using built-in Qt Vector and StringList deserialization
		if (!docFileStream.atEnd())
			docFileStream >> editors >> _text;

		if (docFileStream.status() != QDataStream::Status::Ok)
		{
			// THROW: handle error or throw DocumentLoadException ?
		}

		file.close();

		std::cout << "done" << std::endl;
	}
	else
	{
		QFileInfo info(file);
		//throw FileLoadException(info.absolutePath().toStdString());
	}
}

void Document::unload()
{
	// Unload the Document object contents from memory
	editors.clear();
	_text.clear();
	_text.squeeze();		// release allocated but unused memory until the document gets reloaded
}

void Document::save()
{
	// Create or overwrite the document file on disk, and write data to it
	QFile file(DOCUMENTS_DIRNAME + ("tmp_" + uri.toString()));
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream docFileStream(&file);

		std::cout << "\nSaving document \"" << uri.toStdString() << "\"... ";

		// Write the the current document content to file
		// using built-in Qt Vector and StringList serialization
		docFileStream << editors << _text;

		QFile oldFile(DOCUMENTS_DIRNAME + uri.toString());
		if (oldFile.exists())
		{
			if (oldFile.remove())
				file.rename(DOCUMENTS_DIRNAME + uri.toString());
			else
			{
				// Remove temporary file if overwriting of the old file failed
				file.remove();
				QFileInfo info(oldFile);
				//throw FileOverwriteException(info.absoluteFilePath().toStdString());
			}
		}
		else file.rename(DOCUMENTS_DIRNAME + uri.toString());

		file.close();

		std::cout << "done" << std::endl;
	}
	else
	{
		QFileInfo info(file);
		//throw FileWriteException(info.absolutePath().toStdString(), info.fileName().toStdString());
	}
}


int Document::insert(Symbol& s)
{
	int insertionIndex = -binarySearch(s._fPos);	// should be a negative position index (for a non-existing Symbol)

	if (insertionIndex >= 0)
	{
		// Check if the inserted symbol implies the creation of a new block
		if (s.getChar() == QChar::ParagraphSeparator)
		{
			TextBlock block;

			if (s.getBlockIdentifier() == qMakePair(-1, -1))
			{
				// Create a new TextBlock with locally-generated ID (symbol received from Qt editor)
				block = TextBlock(_blockCounter++, s.getAuthorId(), QTextBlockFormat());
			}
			else
			{
				// Create a TextBlock with the ID carried by the symbol itself (received from another client)
				block = TextBlock(s.getBlockIdentifier(), QTextBlockFormat());

				_blockCounter++;	// (keep the block counter aligned between clients)
			}

			// Insert the new block in the document
			_blocks.insert(block.getIdPair(), block);

			if (insertionIndex < _text.length())	// The paragraph separator is splitting an existing block
			{
				QPair<qint32, qint32> prevBlockId = getBlockAt(insertionIndex);

				// The paragraph delimiter belongs to the block on which it is inserted
				s.assignToBlock(_blocks[prevBlockId]);
				_text.insert(_text.begin() + insertionIndex, s);	// (insert the symbol in the vector)

				// And any following symbol of that paragraph is assigned to the new block
				for (int i = insertionIndex + 1;
					i < _text.length() && _text[i].getBlockIdentifier() == prevBlockId;
					i++)
				{
					_text[i].assignToBlock(block);
				}
			}
			else	// The paragraph separator marks the first block in the document
			{
				s.assignToBlock(block);
				_text.insert(_text.begin() + insertionIndex, s);	// (insert the symbol in the vector)
			}
		}
		else
		{
			// Assign the character to the block on which it is inserted
			QPair<qint32, qint32> blockId = getBlockAt(insertionIndex);
			s.assignToBlock(_blocks[blockId]);
			_text.insert(_text.begin() + insertionIndex, s);	// place the symbol in the vector
		}
	}

	return insertionIndex;
}

void Document::remove(const Symbol& s)
{
	this->removeAt(s._fPos);
}

int Document::removeAt(QVector<qint32> fPos)
{
	int pos = binarySearch(fPos);	// looks for the symbol with that fractional position
	if (pos >= 0)
	{
		// Check if the symbol removal implies the deletion of a paragraph separator
		if (_text[pos].getChar() == QChar::ParagraphSeparator && pos < _text.length()-1)
		{
			TextBlock mergedBlock = _blocks[_text[pos].getBlockIdentifier()];

			// The paragraph following the deleted ParagraphSeparator will disappear
			QPair<qint32, qint32> otherBlockId = _text[pos + 1].getBlockIdentifier();

			_text.removeAt(pos);	// Removes the paragraph separator
			mergedBlock.decrementSymbols();

			// All symbols belonging to the next block will be assigned to the current block
			for (int i = pos;
				i < _text.length() && _text[i].getChar() != QChar::ParagraphSeparator;
				i++)
			{
				_text[pos].assignToBlock(mergedBlock);
			}

			_blocks.remove(otherBlockId);		// Delete the (now empty) block from the document
		}
		else if (_text[pos].getChar() == QChar::ParagraphSeparator && pos == _text.length())
		{
			// Deletes the block which is now empty
			_blocks.remove(_text[pos].getBlockIdentifier());

			_text.removeAt(pos);	// Removes the paragraph separator
		}
		else
		{
			_blocks[_text[pos].getBlockIdentifier()].decrementSymbols();
			_text.removeAt(pos);	// Remove the symbol from the document
		}
	}

	return pos;
}

QVector<qint32> Document::removeAtIndex(int index)
{
	QVector<qint32> position = _text[index]._fPos;
	
	removeAt(position);
	return position;
}

int Document::formatBlock(QPair<qint32, qint32> id, QTextBlockFormat fmt)
{
	TextBlock block = _blocks[id];		// find the desired block
	block.setFormat(fmt);				// change the format

	_blocks.insert(id, block);		// and replace its entry with the updated one

	return getBlockPosition(id);
}


QString Document::toString()
{
	QString text;

	for (Symbol* i = _text.begin(); i != _text.end(); i++)
	{
		text.append(i->getChar());
	}

	return text;
}


QPair<qint32, qint32> Document::getBlockAt(int index)
{
	return _text[index].getBlockIdentifier();
}

int Document::getBlockPosition(QPair<qint32, qint32> blockIdPair)
{
	int i = 0;
	while (i < _text.length())
	{
		QPair<qint32, qint32> curBlockId = _text[i].getBlockIdentifier();
		if (curBlockId == blockIdPair)
			return i;
		else i += _blocks[curBlockId].size();
	}

	return -1;
}



// Binary search algorithm, returns the index of the symbol at the specified
// fractional position inside the document, otherwise negative index of where it should be
int Document::binarySearch(QVector<qint32> position)
{
	int lower = 0;
	int higher = _text.size() - 1;
	int m = 0;

	while (lower <= higher)
	{
		m = std::floor((lower + higher) / 2);

		if (_text[m]._fPos == position)
			return m;
		else if (_text[m]._fPos < position)
			m = lower = m + 1;
		else  // _text[m]._fPos > position
			higher = m - 1;
	}

	return -m;		// no match found, negative position returned
}



QVector<qint32> Document::fractionalPosBetween(int prev_i, int next_i)
{
	QVector<qint32> result;
	Symbol prev = _text[prev_i];
	Symbol next = _text[next_i];
	int maxlen = std::max<int>(prev._fPos.size(), next._fPos.size());

	for (int i = 0; i < maxlen; i++)
	{
		int a = i < prev._fPos.size() ? prev._fPos[i] : 0;
		int b = i < next._fPos.size() ? next._fPos[i] : 0;

		if (a < b)
		{
			if (std::abs(b - a) == 1)				// if the elements' fPos are separated by only 1, add a new level 
			{										// and make sure it's higher than the value of prev.fPos[i + 1]
				result.push_back(a);
				int next_a = i + 1 < prev._fPos.size() ? prev._fPos[i + 1] : 0;

				// the gap between the fPos values is increased in deeper levels to avoid making the vector
				// become too long with subsequent insertions in between elements
				result.push_back(next_a + (i + 1) * fPosGapSize);
			}
			else result.push_back((a + b) / 2);		// if the gap is wide enough, choose the middle value between the two

			break;
		}
		else result.push_back(a);	// if the two values are the same, append them to then new fPos and iterate again
	}

	return result;
}

QVector<qint32> Document::fractionalPosAtIndex(int index)
{
	return fractionalPosBetween(index - 1, index);
}

int Document::length()
{
	return _text.length();
}

QVector<qint32> Document::fractionalPosBegin()
{
	QVector<qint32> result;

	// Returns a fractional position which precedes the first symbol's fPos[0] by fPosGapSize 
	if (!_text.empty())
	{
		Symbol& begin = _text.front();
		result.push_back(begin._fPos[0] - fPosGapSize);
	}
	else result.push_back(0);	// or a 0 if it's the first character in the text

	return result;
}

QVector<qint32> Document::fractionalPosEnd()
{
	QVector<qint32> result;

	// Returns a fractional position which follows the last symbol's fPos[0] by fPosGapSize 
	if (!_text.empty())
	{
		Symbol& end = _text.back();
		result.push_back(end._fPos[0] + fPosGapSize);
	}
	else result.push_back(0);	// or a 0 if the text is currently empty

	return result;
}


// TODO: update serialization operators with all new Document fields

QDataStream& operator>>(QDataStream& in, Document& doc)
{
	// Deserialization
	in >> doc.uri >> doc.editors >> doc._blockCounter >> doc._blocks /* >> _lists */ >> doc._text ;

	return in;
}

QDataStream& operator<<(QDataStream& out, const Document& doc)
{
	// Serialization
	out << doc.uri << doc.editors << doc._blockCounter << doc._blocks /* << _lists */ << doc._text;

	return out;
}
