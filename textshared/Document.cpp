#include "Document.h"
#include "SharedException.h"

#include <algorithm>

#include <QDataStream>
#include <QMap>
#include <QFileInfo>
#include <QSaveFile>

#define FPOS_GAP_SIZE 4						// Value used in the fractional position algorithm
#define DOCUMENTS_DIRNAME "./Documents/"	// Path on which the document files are stored (on the server)
#define URI_FIELD_SEPARATOR	'_'


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
	: _blockCounter(0), _listCounter(0)
{
}

Document::Document(URI docURI, qint32 authorId) :
	uri(docURI), _blockCounter(0), _listCounter(0)
{
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


/*************** GETTER METHODS ***************/


URI Document::getURI() const
{
	return uri;
}

QString Document::getName() const
{
	return uri.getDocumentName();
}

QString Document::getAuthor() const
{
	return uri.getAuthorName();
}

QVector<Symbol> Document::getContent()
{
	return _text;
}

int Document::length()
{
	return _text.length();
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


Symbol& Document::operator[](QVector<qint32> fPos)
{
	int pos = positionIndex(fPos);
	if (pos >= 0 && pos < _text.length())
		return _text[pos];
	else throw std::out_of_range("The document doesn't contain any symbol with that fractional position");
}

Symbol& Document::operator[](int pos)
{
	if (pos >= 0 && pos < _text.length())
		return _text[pos];
	else throw std::out_of_range("The document doesn't contain any symbol with that index");
}


/************* DOCUMENT FILE METHODS (Server only) *************/


void Document::load()
{
	// Open the document file on disk, and read data from it
	QFile file(DOCUMENTS_DIRNAME + uri.toString());
	if (file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly))
	{
		QDataStream docFileStream(&file);

		// Load the document content from file via deserialization
		if (!docFileStream.atEnd())
			docFileStream >> _blockCounter >> _blocks >> _listCounter >> _lists >> _text;

		if (docFileStream.status() != QDataStream::Status::Ok)
			throw DocumentLoadException(uri.toStdString(), DOCUMENTS_DIRNAME);

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
		docFileStream << _blockCounter << _blocks << _listCounter << _lists << _text;

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

void Document::erase()
{
	// Delete the document from the local file system
	QFile(DOCUMENTS_DIRNAME + uri.toString()).remove();
}



/************ EDITING OPERATIONS ***********/


int Document::insert(Symbol& s)
{
	int insertPos = insertionIndex(s._fPos);	// Search for the insertion position
	assert(insertPos >= 0);

	// Check if the inserted symbol implies the creation of a new block
	if (_text.empty() || (s.getChar() == QChar::ParagraphSeparator && insertPos < _text.length())
		|| (insertPos == _text.length() && _text[insertPos-1].getChar() == QChar::ParagraphSeparator
			&& s.getChar() != QChar::Null))
	{
		QMap<TextBlockID, TextBlock>::iterator block;

		if (!s.getBlockId())	// (symbol received from Qt editor)
		{
			// Create a new TextBlock with locally-generated ID
			TextBlockID blockId(_blockCounter++, s.getAuthorId());
			block = _blocks.insert(blockId, TextBlock(blockId, QTextBlockFormat()));

			s.setBlock(blockId);
		}
		else	// (symbol received from remote)
		{
			// Create a TextBlock with the ID carried by the symbol itself (received from another client)
			block = _blocks.insert(s.getBlockId(), TextBlock(s.getBlockId(), QTextBlockFormat()));

			_blockCounter++;	// (keep the block counter aligned between clients)
		}

		// Check if it's needed to split blocks
		if (s.getChar() == QChar::ParagraphSeparator && !_text.empty() && insertPos != _text.length())
		{
			TextBlock& prevBlock = _blocks[getBlockAt(insertPos)];

			// The paragraph delimiter belongs to the previous block
			s.setBlock(nullptr);
			addCharToBlock(s, prevBlock);
			_text.insert(_text.begin() + insertPos, s);

			// The new paragraph inherits the format attributes from the previous one
			block->setFormat(prevBlock.getFormat());

			// Migrate the list belonging from the previous to the new block
			if (prevBlock.getListId())
			{
				TextList& list = _lists[prevBlock.getListId()];
				addBlockToList(*block, list);
				removeBlockFromList(prevBlock, list);
			}

			// All the following symbols of that paragraph are assigned to the new block
			for (int i = positionIndex(prevBlock.end()); i > insertPos; i--) {
				addCharToBlock(_text[i], *block);
			}

			/* We guarantee that this copy of the symbol contains the ID of the block that was created
			because of its insertion (even though in _text it belongs to another block) */
			s.setBlock(block->getId());
		}
		else
		{
			// Insert the symbol in the document
			addCharToBlock(s, *block);
			_text.insert(_text.begin() + insertPos, s);
		}
	}
	else	// Inserting a regular symbol in the document
	{
		// Assign the character to the block on which it is inserted
		TextBlockID blockId = (insertPos == _text.length() ?
			getBlockAt(insertPos - 1) :	getBlockAt(insertPos));		// the last char belongs to the previous block
		addCharToBlock(s, _blocks[blockId]);
		_text.insert(_text.begin() + insertPos, s);
	}

	return insertPos;
}


int Document::remove(QVector<qint32> fPos)
{
	int pos = positionIndex(fPos);	// looks for the symbol with that fractional position
	if (pos < 0)
		return -1;					// Early out if the symbol has already been deleted

	Symbol& s = _text[pos];
	TextBlock& block = _blocks[s.getBlockId()];

	// Check if the symbol removal implies the merging of two blocks
	if (s.getChar() == QChar::ParagraphSeparator && pos < _text.length() - 1 &&
		!(block.begin() == block.end()))
	{
		// All symbols belonging to the next block will be assigned to the current block
		for (int i = pos + 1; i < _text.length(); i++)
		{
			addCharToBlock(_text[i], block);
			if (_text[i].getChar() == QChar::ParagraphSeparator)
				break;
		}
	}

	// Remove the symbol from the document
	removeCharFromBlock(s, block);
	_text.removeAt(pos);

	return pos;
}

QVector<qint32> Document::removeAtIndex(int index)
{
	if (index < 0 || index >= _text.length())
		throw std::out_of_range("The specified index is not a valid position for the document");

	QVector<qint32> fPosition = _text[index]._fPos;
	
	remove(fPosition);
	return fPosition;
}


int Document::editBlockList(TextBlockID blockId, TextListID listId, QTextListFormat fmt)
{
	// Early out if the operation refers to a non-existing block
	if (!_blocks.contains(blockId))		
		return -1;

	TextBlock& block = _blocks[blockId];

	if (listId)
	{
		if (!_lists.contains(listId))
		{
			// Create the list if the id is unknown
			_lists.insert(listId, TextList(listId, fmt));
			_listCounter++;	  // (to keep list counters aligned across clients)
		}

		// Add the block to the list
		TextList& list = _lists[listId];
		addBlockToList(block, list);
	}
	else
	{
		if (block.getListId())
		{
			// Remove the block from the list
			TextList& list = _lists[block.getListId()];
			removeBlockFromList(block, list);
		}
	}

	return getBlockPosition(blockId);
}


int Document::formatSymbol(QVector<qint32> fPos, QTextCharFormat fmt)
{
	int pos = positionIndex(fPos);		// looks for the symbol with that fractional position

	if (pos >= 0)						// if the symbol exists in the document
		_text[pos].setFormat(fmt);		// change its format with the new one

	return pos;
}

int Document::formatBlock(TextBlockID id, QTextBlockFormat fmt)
{
	QMap<TextBlockID, TextBlock>::iterator block = _blocks.find(id);
	if (block != _blocks.end())
		block->setFormat(fmt);				// update the format (if block exists)
	
	return getBlockPosition(id);
}

int Document::formatList(TextListID id, QTextListFormat fmt)
{
	QMap<TextListID, TextList>::iterator list = _lists.find(id);
	if (list != _lists.end())
		list->setFormat(fmt);				// update list format (if valid listId)

	return getListPosition(id);
}


/*********** BLOCK METHODS **********/


TextBlock& Document::getBlock(TextBlockID id)
{
	return _blocks.find(id).value();
}

int Document::getBlockPosition(TextBlockID blockId)
{
	QMap<TextBlockID, TextBlock>::iterator block = _blocks.find(blockId);
	if (block != _blocks.end())
		return positionIndex(block->begin());
	else return -1;
}

TextBlockID Document::getBlockAt(int index)
{
	if (index < 0 || index >= _text.length())
		throw std::out_of_range("The specified index is not a valid position for the document");

	return _text[index].getBlockId();
}

QList<TextBlockID> Document::getBlocksBetween(int start, int end)
{
	QList<TextBlockID> result;

	// Clamping begin and end indexes to avoid out-of-range
	start = std::clamp<int>(start, 0, _text.length());
	end = std::clamp<int>(end, 0, _text.length());

	int n = start;
	while (n < _text.length())
	{
		TextBlockID blockId = getBlockAt(n);
		TextBlock& block = _blocks[blockId];	// Get the block and add it to the list of results
		result.append(blockId);
		n = positionIndex(block.end()) + 1;		// Skip to the beginning of the next block
		if (n <= 0 || n >= end)
			break;	
	}

	return result;
}


void Document::addCharToBlock(Symbol& s, TextBlock& b)
{
	if (s.getBlockId() && s.getBlockId() != b.getId())
	{
		// Remove the char from any previous block it's in
		removeCharFromBlock(s, _blocks[s.getBlockId()]);
	}

	s.setBlock(b.getId());

	if (b.isEmpty())
	{
		b.setBegin(s._fPos);	// set block delimiters
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
		// Completely remove the block when it's empty
		if (b.getListId()) {
			TextList& list = _lists[b.getListId()];
			removeBlockFromList(b, list);
		}
		_blocks.remove(b.getId());
	}
	else if (s._fPos == b.begin())
	{
		int beginIndex = positionIndex(b.begin());
		assert(beginIndex >= 0);

		b.setBegin(_text[beginIndex + 1]._fPos);
	}
	else if (s._fPos == b.end())
	{
		int endIndex = positionIndex(b.end());
		assert(endIndex >= 0);

		b.setEnd(_text[endIndex - 1]._fPos);
	}
}



/********** LIST METHODS **********/


TextList& Document::getList(TextListID id)
{
	return _lists.find(id).value();
}

int Document::getListPosition(TextListID listId)
{
	// Returns the position of the first block in the list
	QMap<TextListID, TextList>::iterator list = _lists.find(listId);
	if (list != _lists.end())
		return getBlockPosition(list->getBlocks().first());
	else return -1;
}

TextListID Document::getListAt(int index)
{
	if (index < 0 || index >= _text.length())
		throw std::out_of_range("The specified index is not a valid position for the document");

	return _blocks[getBlockAt(index)].getListId();
}

QList<TextBlockID> Document::getOrderedListBlocks(TextListID listId)
{
	QMap<QVector<qint32>, TextBlockID> ordered_blocks;
	QMap<TextListID, TextList>::iterator list = _lists.find(listId);

	if (list != _lists.end())
	{
		// Build an ordered map of the list blocks (sorted by their position in the document)
		// in order to obtain an ordered list of blockIDs by calling .values()
		foreach(TextBlockID blockId, list->getBlocks())
		{
			TextBlock& block = _blocks[blockId];
			ordered_blocks.insert(block.begin(), blockId);
		}
	}

	return ordered_blocks.values();
}


void Document::addBlockToList(TextBlock& b, TextList& l)
{
	if (b.getListId() && b.getListId() != l.getId())
	{
		// Remove the block from any previous list it's in
		removeBlockFromList(b, _lists[b.getListId()]);
	}

	b.setList(l.getId());
	l.addBlock(b.getId());
}

void Document::removeBlockFromList(TextBlock& b, TextList& l)
{
	b.setList(nullptr);
	l.removeBlock(b.getId());

	if (l.isEmpty())
	{
		// Delete the list when it's empty
		_lists.remove(l.getId());
	}
}



/************ BINARY SEARCH ALGORITHM *************/


// Binary search, returns the index of the symbol at the specified fractional position
// otherwise returns -1 (if no symbol has that position) 
int Document::positionIndex(QVector<qint32> position)
{
	int lower = 0;
	int higher = _text.size() - 1;
	int m = 0;

	while (lower <= higher)
	{
		m = std::floor((lower + higher) / 2);

		if (_text[m]._fPos == position)
			return m;							// search hit
		else if (_text[m]._fPos < position)
			m = lower = m + 1;
		else  // _text[m]._fPos > position
			higher = m - 1;
	}

	return -1;		// no match found
}

// Binary search, returns the index at which a new symbol with the specified fPos should be inserted
// otherwise returns -1 (if a symbol with that fractional position already exists) 
int Document::insertionIndex(QVector<qint32> position)
{
	int lower = 0;
	int higher = _text.size() - 1;
	int m = 0;

	while (lower <= higher)
	{
		m = std::floor((lower + higher) / 2);

		if (_text[m]._fPos == position)
			return -1;
		else if (_text[m]._fPos < position)
			m = lower = m + 1;
		else  // _text[m]._fPos > position
			higher = m - 1;
	}

	return m;		// the final index represents the insertion position
}



/************ FRACTIONAL POSITION ALGORITHM *************/


/* Generate a fractional position (an array of values) which should
identify a new symbol inserted in the document at the specified index */
QVector<qint32> Document::newFractionalPos(int index)
{
	QVector<qint32> result;

	if (index < 0 || index > _text.length())
		throw std::out_of_range("The specified index is not a valid position for the document");

	if (_text.empty())		// First character in the document
	{
		result.push_back(0);
	}
	else if (index == 0)		// Beginning of the document
	{
		// The fractional position will precede the first symbol's fPos[0] by FPOS_GAP_SIZE 
		Symbol& begin = _text.front();
		result.push_back(begin._fPos[0] - FPOS_GAP_SIZE);
	}
	else if (index == _text.length())	  // End of the document
	{
		// The fractional position will follow the last symbol's fPos[0] by FPOS_GAP_SIZE 
		Symbol& end = _text.back();
		result.push_back(end._fPos[0] + FPOS_GAP_SIZE);
	}
	else
	{
		Symbol& prev = _text[index - 1];		// The symbols which 'sandwich' the inserion position
		Symbol& next = _text[index];
		int maxlen = std::max<int>(prev._fPos.size(), next._fPos.size());

		for (int i = 0; i < maxlen; i++)
		{
			int a = i < prev._fPos.size() ? prev._fPos[i] : 0;
			int b = i < next._fPos.size() ? next._fPos[i] : 0;

			if (a < b)
			{
				if (std::abs(b - a) == 1)		// if the elements' fPos are separated by only 1, add a new level of depth
				{								// and make sure it's higher than the value of prev.fPos[i + 1]
					result.push_back(a);
					int next_a = i + 1 < prev._fPos.size() ? prev._fPos[i + 1] : 0;

					// the gap between the fPos values is increased in deeper levels to avoid making the vector
					// become too long with subsequent insertions in between elements
					result.push_back(next_a + (i + 1) * FPOS_GAP_SIZE);
				}
				else result.push_back((a + b) / 2);			// if the gap is wide enough, choose the middle value between the two

				break;
			}
			else result.push_back(a);		// if the two values are the same, append them to then new fPos and iterate again
		}
	}

	return result;
}



/********* SERIALIZATION OPERATORS **********/


QDataStream& operator>>(QDataStream& in, Document& doc)		// Input
{
	// Deserialization
	in >> doc.uri >> doc._blockCounter >> doc._blocks 
		>> doc._listCounter >> doc._lists >> doc._text ;

	return in;
}

QDataStream& operator<<(QDataStream& out, const Document& doc)		// Output
{
	// Serialization
	out << doc.uri << doc._blockCounter << doc._blocks 
		<< doc._listCounter << doc._lists << doc._text;

	return out;
}
