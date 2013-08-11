#include "RootBlock.h"
#include "Block.h"

namespace BTRF {

RootBlock::RootBlock(TML::TMLFile *tmlFile) : tmlFile(tmlFile)
{
	//ctor
}

void RootBlock::addBlock(Block *block) {
	blocks.insert(std::pair<TemplateGuid, Block*>(block->getTemplateGuid(), block));
	blockList.push_back(block);
}

void RootBlock::addString(const char* str) {
	stringList.push_back(std::string(str));
}

const char *RootBlock::getString(int index) {
	return stringList.at(index).c_str();
}

void RootBlock::addTemplate(TemplateGuid guid, int usedField) {
	TemplateInfo templateInfo = {guid, usedField};
	templateList.push_back(templateInfo);
}

TemplateGuid RootBlock::getTemplateGuid(int index) {
	return templateList.at(index).guid;
}

int RootBlock::getTemplateUsedField(int index) {
	return templateList.at(index).usedFieldsNum;
}

void RootBlock::dumpToStdout() {
	std::cout << "BTRF/DIFK File\n" << blockList.size() << " root blocks\n";
	for(size_t i = 0; i < blockList.size(); ++i) {
		Block *block = blockList.at(i);
		std::cout << "Block no" << i << "\n{\n";
		block->dumpToStdout();
		std::cout << "}\n";
	}
}

} // namespace BTRF
