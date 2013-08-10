#include "RootBlock.h"
#include "Block.h"

namespace BTRF {

RootBlock::RootBlock()
{
	//ctor
}

void RootBlock::addBlock(Block *block) {
	blocks.insert(std::pair<TemplateGuid, Block*>(block->getTemplateGuid(), block));
	blockList.push_back(block);
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
