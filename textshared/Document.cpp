#include "Document.h"
#include "SharedException.h"

#include <algorithm>

#include <QDataStream>
#include <QMap>
#include <QFileInfo>
#include <QSaveFile>

#define FPOS_GAP_SIZE 4						// Value used in the fractional position algorithm


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
	Symbol eof = Symbol(QChar::ParagraphSeparator, QTextCharFormat(), Position({ 100, 100, authorId }));
	defaultBlock.setBegin(eof.getPosition());
	defaultBlock.setEnd(eof.getPosition());
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

QVector<Symbol> Document::getContent() const
{
	return _text;
}

int Document::length() const
{
	return _text.size();
}

QString Document::toString() const
{
	QString text;

	for (QVector<Symbol>::const_iterator i = _text.begin(); i != _text.end(); i++)
		text.append(i->getChar());

	return text;
}


Symbol& Document::operator[](const Position& fPos)
{
	int pos = findPosition(fPos);
	if (pos >= 0 && pos < _text.size())
		return _text[pos];
	else throw std::out_of_range("The document doesn't contain any symbol with that fractional position");
}

Symbol& Document::operator[](int pos)
{
	if (pos >= 0 && pos < _text.size())
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


int Document::insert(Symbol& s, int hint)
{
	int insertPos = -1;

	//Check if the hint is correct
	if (hint > 0 && hint < _text.size() - 1)
	{
		if (_text[hint - 1].getPosition() < s.getPosition() && s.getPosition() < _text[hint].getPosition())
			insertPos = hint;
	}
	else if (hint == 0 && !_text.isEmpty())
	{
		if (s.getPosition() < _text.first().getPosition())
			insertPos = hint;
	}
	else if (hint == _text.size() && !_text.isEmpty())
	{
		if (s.getPosition() > _text.last().getPosition())
			insertPos = hint;
	}

	if (insertPos < 0)
		insertPos = insertionIndex(s.getPosition());	// Search for the insertion position

	assert(insertPos >= 0);

	// Check if the inserted symbol implies the creation of a new block
	if (_text.empty() || (s.getChar() == QChar::ParagraphSeparator && insertPos < _text.size())
		|| (insertPos == _text.size() && _text[insertPos - 1].getChar() == QChar::ParagraphSeparator
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
		if (s.getChar() == QChar::ParagraphSeparator && !_text.empty() && insertPos != _text.size())
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
			for (int i = findPosition(prevBlock.end()); i > insertPos; i--) {
				addCharToBlock(_text[i], *block);
			}

			// We guarantee that this copy of the symbol contains the ID of the block that was created
			// because of its insertion (even though in _text it belongs to another block)
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
		TextBlockID blockId = (insertPos == _text.size() ?
			getBlockAt(insertPos - 1) : getBlockAt(insertPos));		// the last char belongs to the previous block
		addCharToBlock(s, _blocks[blockId]);
		_text.insert(_text.begin() + insertPos, s);
	}

	return insertPos;
}


int Document::remove(const Position& fPos, int hint)
{
	int pos = -1;

	//Check if the hint is correct
	if (hint >= 0 && hint < _text.size() - 1 && _text[hint].getPosition() == fPos)
	{
		pos = hint;
	}
	else
	{
		pos = findPosition(fPos);		// looks for the symbol with that fractional position
		if (pos < 0)
			return -1;					// Early out if the symbol has already been deleted
	}

	Symbol& s = _text[pos];
	TextBlock& block = _blocks[s.getBlockId()];

	// Check if the symbol removal implies the merging of two blocks
	if (s.getChar() == QChar::ParagraphSeparator && pos < _text.size() - 1 &&
		!(block.begin() == block.end()))
	{
		// All symbols belonging to the next block will be assigned to the current block
		for (int i = pos + 1; i < _text.size(); i++)
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

Position Document::removeAtIndex(int index)
{
	if (index < 0 || index >= _text.size())
		throw std::out_of_range("The specified index is not a valid position for the document");

	Position fPosition = _text[index].getPosition();
	
	remove(fPosition, index);
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


int Document::formatSymbol(const Position& fPos, QTextCharFormat fmt, int hint)
{
	int pos = -1;

	//Check if the hint is correct
	if (hint >= 0 && hint < _text.size() - 1 && _text[hint].getPosition() == fPos)
	{
		pos = hint;
	}
	else
	{
		pos = findPosition(fPos);		// looks for the symbol with that fractional position
		if (pos < 0)
			return -1;					// Early out if the symbol does not exist in the document
	}

	_text[pos].setFormat(fmt);		// replace the char format with the new one
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
		return findPosition(block->begin());
	else return -1;
}

TextBlockID Document::getBlockAt(int index)
{
	if (index < 0 || index >= _text.size())
		throw std::out_of_range("The specified index is not a valid position for the document");

	return _text[index].getBlockId();
}

QList<TextBlockID> Document::getBlocksBetween(int start, int end)
{
	QList<TextBlockID> result;

	// Clamping begin and end indexes to avoid out-of-range
	start = std::clamp<int>(start, 0, _text.size());
	end = std::clamp<int>(end, 0, _text.size());

	int n = start;
	while (n < _text.size())
	{
		TextBlockID blockId = getBlockAt(n);
		TextBlock& block = _blocks[blockId];	// Get the block and add it to the list of results
		result.append(blockId);
		n = findPosition(block.end()) + 1;		// Skip to the beginning of the next block
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
		b.setBegin(s.getPosition());	// set block delimiters
		b.setEnd(s.getPosition());
	}
	else if (s.getPosition() < b.begin()) {
		b.setBegin(s.getPosition());		 // update block begin
	}
	else if (s.getPosition() > b.end()) {
		b.setEnd(s.getPosition());			 // update block end
	}
}

void Document::removeCharFromBlock(Symbol& s, TextBlock& b)
{
	s.setBlock(nullptr);

	if (s.getPosition() == b.begin() && s.getPosition() == b.end())
	{
		// Completely remove the block when it's empty
		if (b.getListId()) {
			TextList& list = _lists[b.getListId()];
			removeBlockFromList(b, list);
		}
		_blocks.remove(b.getId());
	}
	else if (s.getPosition() == b.begin())
	{
		int beginIndex = findPosition(b.begin());
		assert(beginIndex >= 0);

		b.setBegin(_text[beginIndex + 1].getPosition());
	}
	else if (s.getPosition() == b.end())
	{
		int endIndex = findPosition(b.end());
		assert(endIndex >= 0);

		b.setEnd(_text[endIndex - 1].getPosition());
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
	if (index < 0 || index >= _text.size())
		throw std::out_of_range("The specified index is not a valid position for the document");

	return _blocks[getBlockAt(index)].getListId();
}

QList<TextBlockID> Document::getOrderedListBlocks(TextListID listId)
{
	QMap<Position, TextBlockID> ordered_blocks;
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
int Document::findPosition(const Position& pos)
{
	int lower = 0;
	int higher = _text.size() - 1;
	int m = 0;

	while (lower <= higher)
	{
		m = std::floor((lower + higher) / 2);

		if (_text[m].getPosition() == pos)	 // search hit
			return m;
		else if (_text[m].getPosition() < pos)
			m = lower = m + 1;
		else higher = m - 1;
	}

	return -1;		// no match found
}


// Binary search, returns the index at which a new symbol with the specified fPos should be inserted
// otherwise returns -1 (if a symbol with that fractional position already exists) 
int Document::insertionIndex(const Position& pos)
{
	int lower = 0;
	int higher = _text.size() - 1;
	int m = 0;

	while (lower <= higher)
	{
		m = std::floor((lower + higher) / 2);

		if (_text[m].getPosition() == pos)
			return -1;
		else if (_text[m].getPosition() < pos)
			m = lower = m + 1;
		else  // _text[m].getPosition() > pos
			higher = m - 1;
	}

	return m;		// the final index represents the insertion position
}


/************ FRACTIONAL POSITION ALGORITHM *************/


/* Generate a fractional position (an array of values) which should identify
 a new symbol inserted in the document at the specified index by a certain user */
Position Document::newFractionalPos(int index, qint32 authorId)
{
	QVector<qint32> result;

	if (index < 0 || index > _text.size())
		throw std::out_of_range("The specified index is not a valid position for the document");

	if (_text.empty())		// First character in the document
	{
		result.push_back(0);
	}
	else if (index == 0)		// Beginning of the document
	{
		// The fractional position will precede the first symbol's fPos[0] by FPOS_GAP_SIZE 
		const Position& begin = _text.first().getPosition();
		result.push_back(begin[0] - FPOS_GAP_SIZE);
	}
	else if (index == _text.size())	  // End of the document
	{
		// The fractional position will follow the last symbol's fPos[0] by FPOS_GAP_SIZE 
		const Position& end = _text.last().getPosition();
		result.push_back(end[0] + FPOS_GAP_SIZE);
	}
	else
	{
		const Position& prev = _text[index - 1].getPosition();		// The symbols which 'sandwich' the inserion position
		const Position& next = _text[index].getPosition();
		int maxlen = std::max<int>(prev.size(), next.size());

		for (int i = 0; i < maxlen; i++)
		{
			int a = i < prev.size() ? prev[i] : 0;
			int b = i < next.size() ? next[i] : 0;

			if (a < b)
			{
				if (std::abs(b - a) == 1)		// if the elements' fPos are separated by only 1, add a new level of depth
				{								// and make sure it's higher than the value of prev.fPos[i + 1]
					result.push_back(a);
					int next_a = i + 1 < prev.size() ? prev[i + 1] : 0;

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

	result.push_back(authorId);		// User ID is added as part of the fractional position to ensure uniqueness
	return Position(result);
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
