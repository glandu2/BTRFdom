/*
 * BTRFdom - Rappelz BTRF Document Object Model
 * By Glandu2
 * Copyright 2013 Glandu2
 *
 * This file is part of BTRFdom.
 * BTRFdom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BTRFdom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with BTRFdom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BtrfRootBlock.h"
#include "BtrfBlock.h"
#include <stdio.h>

BtrfRootBlock::BtrfRootBlock(TmlFile *tmlFile) : tmlFile(tmlFile)
{
	//ctor
	marker = "BtrfRootBlock";
}

BtrfRootBlock::~BtrfRootBlock()
{
	//ctor
	marker = "NotABtrfRootBlock";
}

int BtrfRootBlock::addBlock(IBtrfBlock *iBlock) {
	BtrfBlock* block = static_cast<BtrfBlock*>(iBlock);

	if(block->getTemplateId() == -1) {
		for(unsigned int i = 0; i < templateList.size(); i++) {
			if(templateList.at(i).guid == block->getFieldInfo()->getTemplateGuid()) {
				block->setTemplateId(i);
				break;
			}
		}
		if(block->getTemplateId() == -1)
			block->setTemplateId(addTemplate(block->getFieldInfo()->getTemplateGuid(), block->getElementNumber()));
	}

	blocks.insert(std::pair<TemplateGuid, BtrfBlock*>(block->getTemplateGuid(), block));
	blockList.push_back(block);
	return (int)blockList.size() - 1;
}

int BtrfRootBlock::addString(const char* str) {
	stringList.push_back(std::string(str));
	return (int)stringList.size() - 1;
}

const char *BtrfRootBlock::getString(int index) {
	if(index < 0 || index >= (int)stringList.size()) {
		std::cerr << "Error: invalid string index: " << index << "but there is " << stringList.size() << "registered strings\n";
		exit(-2);
	}
	return stringList.at(index).c_str();
}

int BtrfRootBlock::addTemplate(const TemplateGuid& guid, int usedField) {
	TemplateInfo templateInfo = {guid, usedField};
	templateList.push_back(templateInfo);
	return (int)templateList.size() - 1;
}

const TemplateGuid& BtrfRootBlock::getTemplateGuid(int index) {
	if(index < 0 || index >= (int)templateList.size()) {
		std::cerr << "Error: invalid template index: " << index << " but there is " << templateList.size() << " registered templates\n";
		exit(-2);
	}
	return templateList.at(index).guid;
}

int BtrfRootBlock::getTemplateUsedField(int index) {
	if(index < 0 || index >= (int)templateList.size()) {
		std::cerr << "Error: invalid template index: " << index << " but there is " << templateList.size() << " registered templates\n";
		exit(-2);
	}
	return templateList.at(index).usedFieldsNum;
}

BtrfBlock* BtrfRootBlock::getBlockByGuid(const TemplateGuid& guid) {
	std::unordered_multimap<TemplateGuid, BtrfBlock*>::const_iterator it;
	it = blocks.find(guid);
	if(it != blocks.end())
		return it->second;
	else
		return 0;
}

void BtrfRootBlock::dumpToStdout(FILE *fout) {
	fprintf(fout, "BTRF/DIFK File\n%zd root blocks\n", blockList.size());
	for(size_t i = 0; i < blockList.size(); ++i) {
		BtrfBlock *block = blockList.at(i);
		fprintf(fout, "Block no %zd\n{\n", i);
		block->dumpToStdout(fout, 1);
		fprintf(fout, "}\n");
	}
}
