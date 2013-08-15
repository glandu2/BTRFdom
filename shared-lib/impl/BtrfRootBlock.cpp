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

BtrfRootBlock::BtrfRootBlock(TmlFile *tmlFile) : tmlFile(tmlFile)
{
	//ctor
}

int BtrfRootBlock::addBlock(IBtrfBlock *block) {
	blocks.insert(std::pair<TemplateGuid, BtrfBlock*>(block->getTemplateGuid(), static_cast<BtrfBlock*>(block)));
	blockList.push_back(static_cast<BtrfBlock*>(block));
	return blockList.size() - 1;
}

int BtrfRootBlock::addString(const char* str) {
	stringList.push_back(std::string(str));
	return stringList.size() - 1;
}

const char *BtrfRootBlock::getString(int index) {
	return stringList.at(index).c_str();
}

int BtrfRootBlock::addTemplate(const TemplateGuid& guid, int usedField) {
	TemplateInfo templateInfo = {guid, usedField};
	templateList.push_back(templateInfo);
	return templateList.size() - 1;
}

const TemplateGuid& BtrfRootBlock::getTemplateGuid(int index) {
	return templateList.at(index).guid;
}

int BtrfRootBlock::getTemplateUsedField(int index) {
	return templateList.at(index).usedFieldsNum;
}

void BtrfRootBlock::dumpToStdout() {
	std::cout << "BTRF/DIFK File\n" << blockList.size() << " root blocks\n";
	for(size_t i = 0; i < blockList.size(); ++i) {
		BtrfBlock *block = blockList.at(i);
		std::cout << "Block no" << i << "\n{\n";
		block->dumpToStdout();
		std::cout << "}\n";
	}
}
