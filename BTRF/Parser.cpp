#include "Parser.h"
#include <iostream>
#include <assert.h>
#include "RootBlock.h"
#include "Block.h"

#include "../TML/TMLFile.h"
#include "../TML/Block.h"

namespace BTRF {

RootBlock *Parser::parse(const char* filename) {
	int blockSize, nextHeaderPos;

	static_assert(sizeof(TemplateGuid) == 16, "Wrong type size");
	static_assert(sizeof(char) == 1, "Wrong type size");
	static_assert(sizeof(short) == 2, "Wrong type size");
	static_assert(sizeof(int) == 4, "Wrong type size");
	static_assert(sizeof(float) == 4, "Wrong type size");
	static_assert(sizeof(GlobalHeader) == 12, "Wrong type size");

	file = new File;
	if(file->open(filename) == false)
		return 0;

	header = file->read<GlobalHeader>(sizeof(*header));

	if(strncmp(header->btrf, "BTRF", 4) || header->header_size != 4) {
		std::cerr << "Invalid BTRF file, missing signature\n";
		return nullptr;
	}
	if(header->major_version != 1 || header->minor_version != 1) {
		std::cerr << "Unsupported BTRF version " << header->major_version << '.' << header->minor_version << ", only version 1.1 si supported\n";
		return nullptr;
	}


	rootBlock = new RootBlock(tmlData);

	nextHeaderPos = header->header_size + 8;

	//Read template list
	file->setPos(nextHeaderPos);
	blockSize = *file->read<int>(4);
	nextHeaderPos += blockSize + 4;

	int templateNum = blockSize/18;
	const TemplateGuid *guids = file->read<TemplateGuid>(16*templateNum);
	const short *fieldNumbers = file->read<short>(2*templateNum);

	for(int i = 0; i < templateNum; i++) {
		rootBlock->addTemplate(i, guids[i], fieldNumbers[i]);
	}

	//Read string list
	file->setPos(nextHeaderPos);
	blockSize = *file->read<int>(4);
	nextHeaderPos += blockSize + 4;

	int stringNum = *file->read<int>(4);
	for(int i = 0; i < stringNum; i++) {
		const char *currentString = file->read<char>(0);
		rootBlock->addString(i, currentString);
		file->read<void>(strlen(currentString)+1);
	}

	//Read blocks
	file->setPos(nextHeaderPos);
	blockSize = *file->read<int>(4);
	nextHeaderPos += blockSize + 4;

	int numBlocks = *file->read<int>(4);
	char type;
	short templateIndex;
	for(int i = 0; i<numBlocks; i++) {
		file->read<void>(3);
		type = *file->read<char>(1);
		assert(type == 9);	//Actually we know only about template root blocks
		file->read<int>(4);	//blockSize
		templateIndex = *file->read<short>(2) - 1;

		TML::Block *templateField = rootBlock->getTmlFile()->getTemplate(rootBlock->getTemplateGuid(templateIndex));
		templateField->setFieldCount(rootBlock->getTemplateUsedField(templateIndex));
		Block *block = new Block;
		if(!parseSubBlock(block, templateField)) {
			std::cerr << "Premature end of file\n";
			break;
		}
		rootBlock->addBlock(block);
	}

	return rootBlock;
}

Block *Parser::parseSubBlock(Block *block, TML::Block *tmlField) {
	int blockSize;
	int i;
	short templateIndex;
	Block *subBlocks;

	ElementType elementType = tmlField->getType();
	block->setFieldInfo(tmlField);
	block->setElementNumber(block->getFieldInfo()->getFieldCount());

	if(elementType == ET_TemplateArray || !block->getElementNumber())
		blockSize = *file->read<int>(4);

	switch(elementType) {
	case ET_TemplateArray:
		if(!block->getElementNumber()) {
			templateIndex = *file->read<short>(2) - 1;

			block->setTemplateGuid(rootBlock->getTemplateGuid(templateIndex));
			TML::Block *templateField = block->getFieldInfo()->getField(0);
			templateField->setFieldCount(rootBlock->getTemplateUsedField(templateIndex));

			block->setElementNumber(*file->read<int>(4));

			subBlocks = new Block[block->getElementNumber()];
			block->setData(subBlocks);
			for(i = 0; i < block->getElementNumber(); i++) {
				if(templateField->getHasVariableSize())
					file->read<int>(4);	//elementSize
				parseSubBlock(&subBlocks[i], templateField);
			}
		} else {
			subBlocks = new Block[block->getElementNumber()];
			block->setData(subBlocks);
			for(i = 0; i < block->getElementNumber(); i++)
				parseSubBlock(&subBlocks[i], block->getFieldInfo()->getField(0));
		}
		break;

	case ET_Template:
		block->setTemplateGuid(block->getFieldInfo()->getTemplateGuid());
		subBlocks = new Block[block->getElementNumber()];
		block->setData(subBlocks);

		for(i = 0; i < block->getElementNumber(); i++) {
			parseSubBlock(&subBlocks[i], block->getFieldInfo()->getField(i));
		}
		break;

	case ET_Char:
	case ET_UChar:
		if(!block->getElementNumber()) {
			file->read<char>(1);	//type
			block->setElementNumber((blockSize-1) / sizeof(char));
		}
		block->setData(const_cast<void*>(file->read<void>(block->getElementNumber()*sizeof(char))));
		break;

	case ET_Word:
		if(!block->getElementNumber()) {
			file->read<char>(1);	//type
			block->setElementNumber((blockSize-1) / sizeof(short));
		}
		block->setData(const_cast<void*>(file->read<void>(block->getElementNumber()*sizeof(short))));
		break;

	case ET_DWord:
		if(!block->getElementNumber()) {
			file->read<char>(1);	//type
			block->setElementNumber((blockSize-1) / sizeof(int));
		}
		block->setData(const_cast<void*>(file->read<void>(block->getElementNumber()*sizeof(int))));
		break;

	case ET_Float:
		if(!block->getElementNumber()) {
			file->read<char>(1);	//type
			block->setElementNumber((blockSize-1) / sizeof(float));
		}
		block->setData(const_cast<void*>(file->read<void>(block->getElementNumber()*sizeof(float))));
		break;

	case ET_String:
		if(!block->getElementNumber()) {
			file->read<char>(1);	//type
			block->setElementNumber((blockSize-1) / sizeof(int));
		}
		const char **string = new(std::nothrow) const char*[block->getElementNumber()];
		block->setData(string);
		for(i = 0; i < block->getElementNumber(); i++) {
			int stringId = *file->read<int>(4);
			if(stringId == 0)
				string[i] = 0;
			else {
				string[i] = rootBlock->getString(stringId-1);
			}
		}
		break;
	}

	return block;
}

} // namespace BTRF
