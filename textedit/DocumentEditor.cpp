#include "DocumentEditor.h"


DocumentEditor::DocumentEditor(Document doc, TextEdit* editor, User& user, QObject* parent)
	: QObject(parent), _document(doc), _textedit(editor), _user(user)
{
	qRegisterMetaType<Position>("Position");
	qRegisterMetaType<TextBlockID>("TextBlockID");
	qRegisterMetaType<TextBlock>("TextBlock");
	qRegisterMetaType<TextListID>("TextListID");
	qRegisterMetaType<TextList>("TextList");
	qRegisterMetaType<Symbol>("Symbol");
	qRegisterMetaType<QTextBlockFormat>("QTextBlockFormat");
	qRegisterMetaType<QTextCharFormat>("QTextCharFormat");
	qRegisterMetaType<QTextListFormat>("QTextListFormat");
	qRegisterMetaType<QVector<Symbol>>("QVector<Symbol>");
	qRegisterMetaType<QVector<Position>>("QVector<Position>");
}


void DocumentEditor::openDocument()
{
	// Insert all symbols in the document
	/*for (int i = 0; i < _document.length() - 1; i++) {
		_textedit->newChar(_document[i].getChar(), _document[i].getFormat(), i);
	}*/

	QString buffer;
	QVector<Symbol> text = _document.getContent();
	QVector<Symbol>::iterator s = text.begin();
	int position = 0;
	QTextCharFormat oldFmt;

	for (; s < text.end(); s++)
	{
		if (oldFmt == s->getFormat())
		{
			buffer.append(s->getChar());
		}
		else
		{
			_textedit->manyChars(buffer, oldFmt, position);
			position += buffer.length();
			buffer.clear();
			buffer.append(s->getChar());
			oldFmt = s->getFormat();
		}
	}

	if (!buffer.isEmpty())
	{
		_textedit->manyChars(buffer, oldFmt, position);
	}


	// Create lists
	foreach(TextList list, _document._lists.values()) 
	{
		QList<TextBlockID> blocks = list.getBlocks();
		int firstListBlockPosition = _document.getBlockPosition(blocks.first());

		_textedit->createList(firstListBlockPosition, list.getFormat());
		blocks.removeFirst();

		foreach(TextBlockID id, blocks) {
			_textedit->addBlockToList(_document.getBlockPosition(id), firstListBlockPosition);
		}
	}

	// Apply block-specific formats
	foreach(TextBlock block, _document._blocks.values()) {
		_textedit->applyBlockFormat(_document.getBlockPosition(block.getId()), block.getFormat());
	}

	_textedit->setCurrentFileName(_document.getName(), _document.getURI().toString());
	_textedit->startCursorTimer();
}


/************ CHAR OPERATIONS ************/

void DocumentEditor::addSymbol(Symbol s, bool isLast)	// REMOTE
{
	int position = _document.insert(s);
	if (!isLast) 
	{	// skip inserting the terminating char in the Qt document, because it has one already
		_textedit->newChar(s.getChar(), s.getFormat(), position);
		_textedit->updateUsersSelections();
	}
}

void DocumentEditor::removeSymbol(Position position)	// REMOTE
{
	int pos = _document.remove(position);
	if (pos >= 0) {
		_textedit->removeChar(pos);
		_textedit->updateUsersSelections();
	}
}


void DocumentEditor::addCharAtIndex(QChar ch, QTextCharFormat fmt, int position, bool isLast)	// LOCAL
{
	Symbol s(ch, fmt, _document.newFractionalPos(position, _user.getUserId()));

	_document.insert(s);
	emit charAdded(s, isLast);
}

void DocumentEditor::deleteCharAtIndex(int position)	// LOCAL
{
	if (position < 0 || position >= _document.length())		// Skip if out-of-range (needed ?)
		return;

	Position fractionalPosition = _document.removeAtIndex(position);
	emit charDeleted(fractionalPosition);
}


void DocumentEditor::bulkInsert(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt)
{
	/*QVector<Symbol>::iterator s = symbols.begin();

	for (; s < symbols.end() - 1; s++)
	{
		int position = _document.insert(*s);
		_textedit->newChar(s->getChar(), s->getFormat(), position);
	}

	if (s < symbols.end())	// Handle the last symbol separately (to avoid checking isLast at every iteration)
	{
		int position = _document.insert(*s);
		if (!isLast)
		{
			// skip inserting the terminating char in the Qt document
			_textedit->newChar(s->getChar(), s->getFormat(), position);
		}
	}

	int blockPos = _document.formatBlock(bId, blkFmt);		// Format the block according to the received QTextBlockFormat
	if (blockPos >= 0) {
		_textedit->applyBlockFormat(blockPos, blkFmt);
	}*/

	QString buffer;
	QVector<Symbol>::iterator s = symbols.begin();
	int firstPosition = 0;
	int oldPosition = -1;
	QTextCharFormat oldFmt;

	for (; s < symbols.end() - 1; s++)
	{
		int position = _document.insert(*s);
		if (position == oldPosition + 1 && oldFmt == s->getFormat())
		{
			buffer.append(s->getChar());
			oldPosition = position;
		}
		else if (oldPosition == -1)
		{
			buffer = QString(s->getChar());
			oldFmt = s->getFormat();
			firstPosition = oldPosition = position;
		}
		else
		{
			_textedit->manyChars(buffer, oldFmt, firstPosition);
			buffer.clear();
			buffer.append(s->getChar());
			oldFmt = s->getFormat();
			oldPosition = position;
		}
	}

	if (s < symbols.end())	// Handle the last symbol separately (to avoid checking isLast at every iteration)
	{
		int position = _document.insert(*s);
		if (!isLast)
		{
			// skip inserting the terminating char in the Qt document
			if (position == oldPosition + 1 && s->getFormat() == oldFmt)
			{
				buffer.append(s->getChar());
				_textedit->manyChars(buffer, oldFmt, firstPosition);
			}
			else
			{
				_textedit->newChar(s->getChar(), s->getFormat(), position);
			}
		}
		else if (!buffer.isEmpty())
		{
			_textedit->manyChars(buffer, oldFmt, firstPosition);
		}
	}

	//_textedit->updateUsersSelections();		// Only once after applying the bulk of changes
}

void DocumentEditor::bulkDelete(QVector<Position> positions)
{
	int position = -1;
	int count = 0;

	for each (Position pos in positions)
	{
		int index = _document.remove(pos);

		if (index >= 0)	  // Skip non-extisting characters
		{
			if (position == -1)
			{
				position = index;
				count = 1;
			}
			else if (index == position)
			{
				count++;
			}
			else
			{
				_textedit->deleteManyChars(position, position + count);
				position = index;
				count = 1;
			}
		}
	}

	if (position >= 0 && count > 0)
	{
		_textedit->deleteManyChars(position, position + count);
	}

	//_textedit->updateUsersSelections();		// This can be done only once after removing all the specified symbols
}

void DocumentEditor::addCharGroupAtIndex(QVector<QChar> chars, QVector<QTextCharFormat> fmts,
	int pos, bool isLast, QTextBlockFormat blkFmt)
{
	QVector<Symbol> symbols;
	QVector<QChar>::iterator i = chars.begin();
	QVector<QTextCharFormat>::iterator j = fmts.begin();

	// Add all the provided chars to the document as symbols and to the list
	// of symbols that will have to be inserted by other clients
	for (int n = 0; i < chars.end() && j < fmts.end(); i++, j++, n++)
	{
		Symbol s(*i, *j, _document.newFractionalPos(pos + n, _user.getUserId()));
		symbols.append(s);

		_document.insert(s);
	}

	TextBlockID blkId = _document.getBlockAt(pos);		// Format the block with the provided QTextBlockFormat
	_document.formatBlock(blkId, blkFmt);
	
	emit charGroupInserted(symbols, isLast, blkId, blkFmt);
}

void DocumentEditor::deleteCharGroupAtIndex(int position, int charCount)
{
	QVector<Position> fPositions;

	for (int i = 0; i < charCount && position < _document.length(); i++)
	{
		// Delete the symbols one by one from the document, inserting their fractional positions
		// in the list of those that need to be removed by other clients
		fPositions.append(_document.removeAtIndex(position));
	}

	emit charGroupDeleted(fPositions);
}


void DocumentEditor::changeSymbolFormat(int position, QTextCharFormat fmt)		// LOCAL
{
	Symbol& s = _document[position];

	_document.formatSymbol(s.getPosition(), fmt);
	emit symbolFormatChanged(s.getPosition(), fmt);
}

void DocumentEditor::applySymbolFormat(Position position, QTextCharFormat fmt)	 // REMOTE
{
	// Early out if the local state is already aligned with the server's
	if (_document[position].getFormat() == fmt)
		return;

	int pos = _document.formatSymbol(position, fmt);
	if (pos >= 0) {
		_textedit->applyCharFormat(pos, fmt);
	}
}



//Generating extra selections for user
void DocumentEditor::generateExtraSelection()
{
	if (_document.length() > 0) {
		QPair<int, int> selectionDelimiters;

		//Get firs character author
		qint32 userId = _document[0].getAuthorId();
		selectionDelimiters.first = 0;
		selectionDelimiters.second = 0;

		//Increment selection upper limit until we reach end of document or a char inserted by another user
		for (int i = 0; i < _document.length() - 1; i++) {
			if (_document[i].getAuthorId() != userId) {
				_textedit->setExtraSelections(userId, selectionDelimiters);

				userId = _document[i].getAuthorId();
				selectionDelimiters.first = i;
				selectionDelimiters.second = i;
			}
			selectionDelimiters.second++;
		}

		_textedit->setExtraSelections(userId, selectionDelimiters);
	}
}



/************ BLOCK OPERATIONS ************/

void DocumentEditor::changeBlockAlignment(int start, int end, Qt::Alignment alignment)		// LOCAL
{
	// Apply the format separately to each block of the selection
	foreach(TextBlockID textBlock, _document.getBlocksBetween(start, end))		
	{
		QTextBlockFormat fmt = _document.getBlock(textBlock).getFormat();
		fmt.setAlignment(alignment);

		_document.formatBlock(textBlock, fmt);
		emit blockFormatChanged(textBlock, fmt);
	}
}

void DocumentEditor::changeBlockLineHeight(int start, int end, qreal height, int heightType)	// LOCAL
{
	foreach(TextBlockID textBlock, _document.getBlocksBetween(start, end))
	{
		QTextBlockFormat fmt = _document.getBlock(textBlock).getFormat();
		fmt.setLineHeight(height, heightType);

		_document.formatBlock(textBlock, fmt);
		emit blockFormatChanged(textBlock, fmt);
	}
}

void DocumentEditor::changeBlockFormat(int start, int end, QTextBlockFormat fmt)	// LOCAL
{
	// Overwrites the current block format for each block in the selection
	foreach(TextBlockID textBlock, _document.getBlocksBetween(start, end)) 
	{
		_document.formatBlock(textBlock, fmt);
		emit blockFormatChanged(textBlock, fmt);
	}
}

void DocumentEditor::applyBlockFormat(TextBlockID blockId, QTextBlockFormat fmt)	// REMOTE
{
	// Early out if the local state is already aligned with the server's
	if (_document.getBlock(blockId).getFormat() == fmt)
		return;

	int position = _document.formatBlock(blockId, fmt);
	if (position >= 0) {
		_textedit->applyBlockFormat(position, fmt);
	}
}


/************ LIST OPERATIONS ************/

void DocumentEditor::listEditBlock(TextBlockID blockId, TextListID listId, QTextListFormat fmt)		// REMOTE
{
	// Early out if the block doesn't exist anymore (locally)
	if (!_document._blocks.contains(blockId))
		return;

	// Early out if the local state is already aligned with the server's
	if (_document.getBlock(blockId).getListId() == listId)
	{
		if (!listId) {	// removeBlockFromList is dispatched anyways, to ensure Qt document consistency between clients
			_textedit->removeBlockFromList(_document.getBlockPosition(blockId));
		}

		return;
	}

	int blockPos = _document.getBlockPosition(blockId);

	if (!listId)
	{
		// Remove the block from its list
		_textedit->removeBlockFromList(blockPos);
	}
	else if (_document._lists.contains(listId))
	{
		// Add the block to an existing list
		_textedit->addBlockToList(blockPos, _document.getListPosition(listId));
	}
	else
	{
		// Create a new list including the specified block
		_textedit->createList(blockPos, fmt);
	}

	// Apply the remote change in the local document
	_document.editBlockList(blockId, listId, fmt);
}


// Called to create a new list that will include the current block
void DocumentEditor::createList(int position, QTextListFormat fmt)		// LOCAL
{
	// Create a new TextList
	TextListID newListId(_document._listCounter++, _user.getUserId());
	_document._lists.insert(newListId, TextList(newListId, fmt));

	TextList& list = _document.getList(newListId);
	TextBlock& block = _document.getBlock(_document.getBlockAt(position));

	_document.addBlockToList(block, list);

	// Notify other clients
	emit blockListChanged(block.getId(), newListId, fmt);
}


// Called by textedit when assigning a recently inserted block to a list
void DocumentEditor::assignBlockToList(int blockPosition, int listPosition)		// LOCAL
{
	// Get the specified block and the list
	TextBlockID blockId = _document.getBlockAt(blockPosition);
	TextListID listId = _document.getListAt(listPosition);

	TextBlock& block = _document.getBlock(blockId);
	TextList& list = _document.getList(listId);

	_document.addBlockToList(block, list);

	// Notify other clients
	emit blockListChanged(blockId, listId, list.getFormat());
}


// Called when a block is set to not be part of a list
void DocumentEditor::removeBlockFromList(int blockPosition)		// LOCAL
{
	// Get the specified block
	TextBlockID blockId = _document.getBlockAt(blockPosition);

	TextBlock& block = _document.getBlock(blockId);
	if (block.getListId())
	{
		TextList& list = _document.getList(block.getListId());
		_document.removeBlockFromList(block, list);	
	}

	// Notify other clients
	emit blockListChanged(blockId, TextListID(nullptr), QTextListFormat());
}


// Handle the press of the list button in the editor
void DocumentEditor::toggleList(int start, int end, QTextListFormat fmt)	// LOCAL
{
	// Get all the blocks inside the user selection
	QList<TextBlockID> selectedBlocks = _document.getBlocksBetween(start, end);

	// Get all the lists involved in the operation (at least one block in the selection)
	QList<TextListID> involvedLists;
	foreach(TextBlockID blockId, selectedBlocks)
	{
		TextBlock& block = _document.getBlock(blockId);
		TextListID blockListId = block.getListId();
		if (blockListId && !involvedLists.contains(blockListId))
			involvedLists.append(blockListId);
	}


	foreach(TextListID listId, involvedLists)
	{
		TextList& oldList = _document.getList(listId);
		TextListID newListId;
		QList<TextBlockID> listBlocks = _document.getOrderedListBlocks(listId);
		bool selectionBegun = false;
		bool selectionEnded = false;

		foreach(TextBlockID blockId, listBlocks)
		{
			TextBlock& block = _document.getBlock(blockId);

			if (!selectionBegun)	// the block is before the selection
			{
				// Iterate on the list blocks until we encounter the first which is part of the selection
				if (selectedBlocks.contains(blockId))
				{
					selectionBegun = true;

					// If the selected blocks are being removed from their lists
					if (fmt.style() == QTextListFormat::ListStyleUndefined)
					{
						_document.removeBlockFromList(block, oldList);

						// Notify both the editor and other clients of the removal
						_textedit->removeBlockFromList(_document.getBlockPosition(blockId));
						emit blockListChanged(blockId, TextListID(nullptr), fmt);
					}
				}
			}
			else if (selectionBegun && !selectionEnded)		// the block among those selected
			{
				if (!selectedBlocks.contains(blockId))
				{
					selectionEnded = true;

					// Create a new list which will contain all the blocks of the old list which come
					// after the end of the user selection; the new list retains its previous format
					newListId = TextListID(_document._listCounter++, _user.getUserId());
					_document._lists.insert(newListId, TextList(newListId, oldList.getFormat()));

					TextList& newList = _document.getList(newListId);
					_document.addBlockToList(block, newList);

					// Apply changes to the editor and notify others
					_textedit->createList(_document.getBlockPosition(blockId), newList.getFormat());
					emit blockListChanged(blockId, newListId, newList.getFormat());
				}
				else
				{
					if (fmt.style() == QTextListFormat::ListStyleUndefined)
					{
						// Remove the block from its list in the local editor and notify server/clients
						_document.removeBlockFromList(block, oldList);
						_textedit->removeBlockFromList(_document.getBlockPosition(blockId));
						emit blockListChanged(blockId, TextListID(nullptr), fmt);
					}
				}
			}
			else	// the block belongs to the list but is after the end of the selection
			{
				// Following blocks are added to the list that was born off the split
				TextList& newList = _document.getList(newListId);
				_document.addBlockToList(block, newList);

				// Apply changes to the editor and notify others
				_textedit->addBlockToList(_document.getBlockPosition(blockId), _document.getListPosition(newListId));
				emit blockListChanged(blockId, newListId, newList.getFormat());
			}
		}
	}


	// If a new list format was applied to the blocks
	if (fmt.style() != QTextListFormat::ListStyleUndefined)
	{
		// Create a new TextList that will contain all the selected blocks
		TextListID newListId(_document._listCounter++, _user.getUserId());
		_document._lists.insert(newListId, TextList(newListId, fmt));
		TextList& list = _document.getList(newListId);

		foreach(TextBlockID blockId, selectedBlocks)
		{
			TextBlock& block = _document.getBlock(blockId);
			if (list.isEmpty())
			{
				// The first block will take care of creating the list in the Qt editor as well
				_textedit->createList(_document.getBlockPosition(blockId), fmt);
			}
			else
			{
				// Following blocks will be appended to the list
				_textedit->addBlockToList(_document.getBlockPosition(blockId), _document.getListPosition(newListId));
			}

			_document.addBlockToList(block, list);

			// Send for each block the message for adding it to the new list
			emit blockListChanged(blockId, newListId, fmt);
		}
	}
}
