#ifndef TML_TMLFILE_H
#define TML_TMLFILE_H

#include <unordered_map>
#include <string>
#include "MemberType.h"

namespace TML {

class Block;
class TMLFile
{
	public:
		TMLFile();
		~TMLFile();

		void parseFile(FILE *file);
		Block* getTemplate(const TemplateGuid& guid);
		Block* getTemplate(const std::string& name);

		void addTemplate(Block *templateStruct);

	private:
		std::unordered_map<TemplateGuid, Block*> templatesByGuid;
		std::unordered_map<std::string, Block*> templatesByName;
};

} // namespace TML

#endif // TML_TMLFILE_H
