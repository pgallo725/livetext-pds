#include "Document.h"
#include "SharedException.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>


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

QString URI::getAuthorName() const
{
	// The first field of the generated URI (fields separated by '_') 
	// is the name of the user which created the document
	return str.section(URI_FIELD_SEPARATOR, 0, 0);
}

QString URI::getDocumentName() const
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

Document::Document(URI docURI, qint32 authorId) :
	uri(docURI), _blockCounter(0)
{
	editors << getAuthor();

	// Insert a ParagraphTerminator character inside a default block in the empty document
	TextBlock defaultBlock = TextBlock(_blockCounter++, authorId, QTextBlockFormat());
	Symbol eof = Symbol(QChar::ParagraphSeparator, QTextCharFormat(),
		authorId, QVector<qint32>({ 1000, 1000 }));
	defaultBlock.setBegin(eof._fPos);
	defaultBlock.setEnd(eof._fPos);
	eof.setBlock(defaultBlock.getId());

	_blocks.insert(defaultBlock.getId(), defaultBlock);
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
	if (file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly))
	{
		QDataStream docFileStream(&file);

		// Load the document content (_text vector<Symbol>) from file
		// using built-in Qt Vector and StringList deserialization
		if (!docFileStream.atEnd())
			docFileStream >> editors >> _blockCounter >> _blocks /* >> _lists */ >> _text;

		if (docFileStream.status() != QDataStream::Status::Ok)
		{	
			throw DocumentLoadException(uri.toStdString(), DOCUMENTS_DIRNAME);
		}

		file.close();
	}
	else
	{
		throw DocumentOpenException(uri.toStdString(), DOCUMENTS_DIRNAME);
	}
}

void Document::unload()
{
	// Unload the Document object contents from memory
	editors.clear();
	_blocks.clear();
	_text.clear();
	_text.squeeze();		// release allocated but unused memory until the document gets reloaded
}

void Document::save()
{
	// Create or overwrite the document file on disk, and write data to it
	QSaveFile file(DOCUMENTS_DIRNAME + uri.toString());
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream docFileStream(&file);

		// Write the the current document content to file
		// using built-in Qt Vector and StringList serialization
		docFileStream << editors << _blockCounter << _blocks /* << _lists */ << _text;

		if (docFileStream.status() == QDataStream::Status::WriteFailed)
		{
			file.cancelWriting();
			file.commit();
			throw DocumentWriteException(uri.toStdString(), DOCUMENTS_DIRNAME);
		}

		file.commit();
	}
	else
	{
		throw DocumentCreateException(uri.toStdString(), DOCUMENTS_DIRNAME);
	}
}

bool Document::exists()
{
	return QFileInfo(QFile(DOCUMENTS_DIRNAME + uri.toString())).exists();
}


Symbol& Document::operator[](QVector<qint32> fPos)
{
	int pos = binarySearch(fPos);
	if (pos >= 0 && pos < _text.length())
		return _text[pos];
	else throw std::out_of_range("The document doesn't contain any symbol with that fractional position");
}


int Document::insert(Symbol& s)
{
	int insertionIndex = -binarySearch(s._fPos);	// should be a negative position index (for a non-existing Symbol)

	if (insertionIndex >= 0)
	{
		qDebug() << "Inserting character" << s.getChar();

		// Check if the inserted symbol implies the creation of a new block
		if (s.getChar() == QChar::ParagraphSeparator)
		{
			if (!s.getBlockId())
			{
				// Create a new TextBlock with locally-generated ID (symbol received from Qt editor)
				TextBlock block = TextBlock(_blockCounter++, s.getAuthorId(), QTextBlockFormat());
				s.setBlock(block.getId());

				// Insert the new block in the document
				_blocks.insert(block.getId(), block);
			}
			else
			{
				// Create a TextBlock with the ID carried by the symbol itself (received from another client)
				_blocks.insert(s.getBlockId(), TextBlock(s.getBlockId(), QTextBlockFormat()));

				_blockCounter++;	// (keep the block counter aligned between clients)
			}

			TextBlock& newBlock = _blocks[s.getBlockId()];
			TextBlock& prevBlock = _blocks[getBlockAt(insertionIndex)];

			// The new block inherits the format from the previous one
			newBlock.setFormat(prevBlock.getFormat());

			Symbol newSymbol(s);

			// The paragraph delimiter belongs to the block on which it is inserted and marks its end
			newSymbol.setBlock(prevBlock.getId());
			prevBlock.setEnd(newSymbol._fPos);
			_text.insert(_text.begin() + insertionIndex, newSymbol);	// (insert the symbol in the vector)

			// And any following symbol of that paragraph is assigned to the new block
			for (int i = insertionIndex + 1; i < _text.length(); i++)
			{
				addCharToBlock(_text[i], newBlock);
				if (_text[i].getChar() == QChar::ParagraphSeparator)	// stop at the first paragraph separator
					break;
			}
		}
		else
		{
			// Assign the character to the block on which it is inserted
			TextBlockID blockId = getBlockAt(insertionIndex);
			addCharToBlock(s, _blocks[blockId]);
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
		Symbol& s = _text[pos];
		qDebug() << "Deleting character" << s.getChar();

		// Check if the symbol removal implies the deletion of a paragraph separator
		if (s.getChar() == QChar::ParagraphSeparator && pos < _text.length()-1)
		{
			TextBlock& mergedBlock = _blocks[s.getBlockId()];

			// The paragraph following the deleted ParagraphSeparator will disappear
			TextBlock& olderBlock = _blocks[_text[pos + 1].getBlockId()];

			removeCharFromBlock(s, mergedBlock);
			_text.removeAt(pos);					// Removes the paragraph separator

			mergedBlock.setEnd(olderBlock.end());

			// All symbols belonging to the next block will be assigned to the current block
			for (int i = pos; i < _text.length(); i++)
			{
				_text[i].setBlock(mergedBlock);
				if (_text[i].getChar() == QChar::ParagraphSeparator)
					break;
			}

			_blocks.remove(olderBlock.getId());		// Delete the (now empty) block from the document
		}
		else if (pos == _text.length() - 1)
		{
			// Avoid deleting the last character in the document (pseudo-EOF symbol)
			// but make sure to reset its format
			_blocks[s.getBlockId()].setFormat(QTextBlockFormat());
		}
		else
		{
			removeCharFromBlock(s, _blocks[s.getBlockId()]);
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

int Document::formatBlock(TextBlockID id, QTextBlockFormat fmt)
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


TextBlock& Document::getBlock(TextBlockID id)
{
	return _blocks[id];
}

int Document::getBlockPosition(TextBlockID blockId)
{
	return binarySearch(_blocks[blockId].begin());
}

TextBlockID Document::getBlockAt(int index)
{
	return _text[index].getBlockId();
}

QList<TextBlockID> Document::getBlocksBetween(int start, int end)
{
	QList<TextBlockID> result;

	int n = start;
	while (n < _text.length())
	{
		TextBlock& block = _blocks[getBlockAt(n)];		// Get the block and add it to the list of results
		result.push_back(block.getId());
		n = binarySearch(block.end()) + 1;		// Skip to the beginning of the next block
		if (n <= 0 || n >= end)
			break;	
	}

	return result;
}


void Document::addCharToBlock(Symbol& s, TextBlock& b)
{
	s.setBlock(b.getId());

	if (b.isEmpty())
	{
		b.setBegin(s._fPos);
		b.setEnd(s._fPos);
	}
	else if (s._fPos < b.begin()) {
		b.setBegin(s._fPos);		 // update block begin
	}
	else if (s._fPos > b.end()) {
		b.setEnd(s._fPos);			 // update block end
	}
}

void Document::removeCharFromBlock(Symbol& s, TextBlock& b)
{
	s.setBlock(nullptr);

	if (s._fPos == b.begin() && s._fPos == b.end())
	{
		b.setBegin(QVector<qint32>({ -1, -1 }));	// Reset the block when deleting the last character
		b.setEnd(QVector<qint32>({ -1, -1 }));
	}
	else if (s._fPos == b.begin())
	{
		int beginIndex = binarySearch(b.begin());
		if (beginIndex >= 0)
			b.setBegin(_text[beginIndex + 1]._fPos);	// update block begin
	}
	else if (s._fPos == b.end())
	{
		int endIndex = binarySearch(b.end());
		if (endIndex >= 0)
			b.setEnd(_text[endIndex - 1]._fPos);	// update block begin
	}
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
