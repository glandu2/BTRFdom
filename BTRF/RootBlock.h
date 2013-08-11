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

		void addString(const char* str);
		const char *getString(int index);

		void addTemplate(TemplateGuid guid, int usedField);
		TemplateGuid getTemplateGuid(int index);
		int getTemplateUsedField(int index);
		int getStringNum() { return stringList.size(); }
		int getTemplateNum() { return templateList.size(); }

		TML::TMLFile *getTmlFile() { return tmlFile; }

		void addBlock(Block *block);
		Block *getBlock(TemplateGuid guid) { try {return blocks.find(guid)->second; } catch(...) { return 0; } }
		Block *getBlock(int id) { return blockList.at(id); }
		int getBlockNum() { return blockList.size(); }

		void dumpToStdout();

	protected:
	private:
		void *fileMemory;
		TML::TMLFile *tmlFile;
		std::deque<Block*> blockList;
		std::unordered_multimap<TemplateGuid, Block*> blocks;
		std::deque<std::string> stringList;
		std::deque<TemplateInfo> templateList;
};

} // namespace BTRF

#endif // BTRF_ROOTBLOCK_H
