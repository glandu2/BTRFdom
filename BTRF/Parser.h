#ifndef BTRF_PARSER_H
#define BTRF_PARSER_H

#include "../File.h"
#include "../TML/MemberType.h"
#include <vector>

namespace TML {
class TMLFile;
class Block;
}

namespace BTRF {

class RootBlock;
class Block;

class Parser
{
	public:
		Parser(TML::TMLFile *tmlData) : tmlData(tmlData) {}

		RootBlock *parse(const char* filename);

	protected:
		Block *parseSubBlock(Block* block, TML::Block *tmlField);

	private:
		struct __attribute__((packed)) GlobalHeader  {
			char btrf[4];
			int header_size;	//always 4
			short major_version;
			short minor_version;
		};

		const GlobalHeader *header;

		struct TemplateInfo {
			int templateNum;
			const TemplateGuid *guids;
			const short *fieldNumbers;
		} templateList;

		File *file;
		TML::TMLFile *tmlData;
		std::vector<const char*> stringList;

};

} // namespace BTRF

#endif // BTRF_PARSER_H
