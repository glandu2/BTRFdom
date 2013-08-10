#ifndef BTRF_ROOTBLOCK_H
#define BTRF_ROOTBLOCK_H

#include <unordered_map>
#include <deque>
#include "../TML/MemberType.h"

namespace TML {
class TMLFile;
}

namespace BTRF {
class Block;

class RootBlock
{
	public:
		RootBlock();

		void addBlock(Block *block);
		Block *getBlock(TemplateGuid guid) { try {return blocks.find(guid)->second; } catch(...) { return 0; } }
		Block *getBlock(int id) { return blockList.at(id); }
		void dumpToStdout();

		friend class Parser;
	protected:
	private:
		void *fileMemory;
		TML::TMLFile *tmlFile;
		std::deque<Block*> blockList;
		std::unordered_multimap<TemplateGuid, Block*> blocks;
};

} // namespace BTRF

#endif // BTRF_ROOTBLOCK_H
