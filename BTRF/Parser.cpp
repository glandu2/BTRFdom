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

	nextHeaderPos = header->header_size + 8;

	//Read template list
	file->setPos(nextHeaderPos);
	blockSize = *file->read<int>(4);
	nextHeaderPos += blockSize + 4;

	templateList.templateNum = blockSize/18;
	templateList.guids = file->read<TemplateGuid>(16*templateList.templateNum);
	templateList.fieldNumbers = file->read<short>(2*templateList.templateNum);

	//Read string list
	file->setPos(nextHeaderPos);
	blockSize = *file->read<int>(4);
	nextHeaderPos += blockSize + 4;

	int stringNum = *file->read<int>(4);
	stringList.resize(stringNum);
	for(int i = 0; i < stringNum; i++) {
		const char* currentString = stringList[i] = file->read<char>(0);
		file->read<void>(strlen(currentString)+1);
	}

	//Read blocks
	file->setPos(nextHeaderPos);
	blockSize = *file->read<int>(4);
	nextHeaderPos += blockSize + 4;

	RootBlock *rootBlock = new RootBlock;

	int numBlocks = *file->read<int>(4);
	char type;
	short templateIndex;
	for(int i = 0; i<numBlocks; i++) {
		file->read<void>(3);
		type = *file->read<char>(1);
		assert(type == 9);	//Actually we know only about template root blocks
		file->read<int>(4);	//blockSize
		templateIndex = *file->read<short>(2) - 1;

		TML::Block *templateField = tmlData->getTemplate(templateList.guids[templateIndex]);
		templateField->setFieldCount(templateList.fieldNumbers[templateIndex]);
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
	block->fieldInfo = tmlField;
	block->numElement = block->fieldInfo->getFieldCount();

	if(elementType == ET_TemplateArray || !block->numElement)
		blockSize = *file->read<int>(4);

	switch(elementType) {
	case ET_TemplateArray:
		if(!block->numElement) {
			templateIndex = *file->read<short>(2) - 1;

			block->templateGuid = templateList.guids[templateIndex];
			TML::Block *templateField = block->fieldInfo->getField(0);
			templateField->setFieldCount(templateList.fieldNumbers[templateIndex]);

			block->numElement = *file->read<int>(4);

			block->data = subBlocks = new Block[block->numElement];
			for(i = 0; i < block->numElement; i++) {
				if(templateField->getHasVariableSize())
					file->read<int>(4);	//elementSize
				parseSubBlock(&subBlocks[i], templateField);
			}
		} else {
			block->data = subBlocks = new Block[block->numElement];
			for(i = 0; i < block->numElement; i++)
				parseSubBlock(&subBlocks[i], block->fieldInfo->getField(0));
		}
		break;

	case ET_Template:
		block->templateGuid = block->fieldInfo->getTemplateGuid();
		block->data = subBlocks = new Block[block->numElement];

		for(i = 0; i < block->numElement; i++) {
			parseSubBlock(&subBlocks[i], block->fieldInfo->getField(i));
		}
		break;

	case ET_Char:
	case ET_UChar:
		if(!block->numElement) {
			file->read<char>(1);	//type
			block->numElement = (blockSize-1) / sizeof(char);
		}
		block->data = const_cast<void*>(file->read<void>(block->numElement*sizeof(char)));
		break;

	case ET_Word:
		if(!block->numElement) {
			file->read<char>(1);	//type
			block->numElement = (blockSize-1) / sizeof(short);
		}
		block->data = const_cast<void*>(file->read<void>(block->numElement*sizeof(short)));
		break;

	case ET_DWord:
		if(!block->numElement) {
			file->read<char>(1);	//type
			block->numElement = (blockSize-1) / sizeof(int);
		}
		block->data = const_cast<void*>(file->read<void>(block->numElement*sizeof(int)));
		break;

	case ET_Float:
		if(!block->numElement) {
			file->read<char>(1);	//type
			block->numElement = (blockSize-1) / sizeof(float);
		}
		block->data = const_cast<void*>(file->read<void>(block->numElement*sizeof(float)));
		break;

	case ET_String:
		if(!block->numElement) {
			file->read<char>(1);	//type
			block->numElement = (blockSize-1) / sizeof(int);
		}
		block->data = new(std::nothrow) const char*[block->numElement];
		for(i=0; i<block->numElement; i++) {
			int stringId = *file->read<int>(4);
			if(stringId == 0)
				static_cast<const char**>(block->data)[i] = 0;
			else {
				static_cast<const char**>(block->data)[i] = stringList[stringId-1];
			}
		}
		break;
	}

	return block;
}

} // namespace BTRF
