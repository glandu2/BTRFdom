#ifndef BTRF_ROOTBLOCK_H
#define BTRF_ROOTBLOCK_H

#include <unordered_map>
#include <deque>
#include <string>
#include "../TML/MemberType.h"

namespace TML {
class TMLFile;
}

namespace BTRF {
class Block;

class RootBlock
{
	public:
		struct TemplateInfo {
			TemplateGuid guid;
			int usedFieldsNum;
		};

	public:
		RootBlock(TML::TMLFile *tmlFile);

		void addString(int index, const char* str);
		const char *getString(int index);

		void addTemplate(int index, TemplateGuid guid, int usedField);
		TemplateGuid getTemplateGuid(int index);
		int getTemplateUsedField(int index);

		TML::TMLFile *getTmlFile() { return tmlFile; }

		void addBlock(Block *block);
		Block *getBlock(TemplateGuid guid) { try {return blocks.find(guid)->second; } catch(...) { return 0; } }
		Block *getBlock(int id) { return blockList.at(id); }
		void dumpToStdout();

	protected:
	private:
		void *fileMemory;
		TML::TMLFile *tmlFile;
		std::deque<Block*> blockList;
		std::unordered_multimap<TemplateGuid, Block*> blocks;
		std::unordered_map<int, std::string> stringList;
		std::unordered_map<int, TemplateInfo> templateList;
};

} // namespace BTRF

#endif // BTRF_ROOTBLOCK_H
