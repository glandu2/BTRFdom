#include "BtrfRootBlock.h"
#include "BtrfBlock.h"

BtrfRootBlock::BtrfRootBlock(TmlFile *tmlFile) : tmlFile(tmlFile)
{
	//ctor
}

void BtrfRootBlock::addBlock(IBtrfBlock *block) {
	blocks.insert(std::pair<TemplateGuid, BtrfBlock*>(block->getTemplateGuid(), static_cast<BtrfBlock*>(block)));
	blockList.push_back(static_cast<BtrfBlock*>(block));
}

void BtrfRootBlock::addString(const char* str) {
	stringList.push_back(std::string(str));
}

const char *BtrfRootBlock::getString(int index) {
	return stringList.at(index).c_str();
}

void BtrfRootBlock::addTemplate(TemplateGuid guid, int usedField) {
	TemplateInfo templateInfo = {guid, usedField};
	templateList.push_back(templateInfo);
}

TemplateGuid BtrfRootBlock::getTemplateGuid(int index) {
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
