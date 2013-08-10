#ifndef DIFK_TEMPLATEDEFINITIONS_H
#define DIFK_TEMPLATEDEFINITIONS_H

#include <vector>
#include <string>
#include "../TML/MemberType.h"

namespace DIFK {

class Block;

class TemplateDefinitions
{
	public:
		struct FieldDef {
			std::string name;
			ElementType type;
			TemplateGuid guid;	//valid only if type == ET_Template
			std::vector<FieldDef*> members;
		};

		TemplateDefinitions();
		~TemplateDefinitions();

		FieldDef* getTemplateFromGuid(TemplateGuid* guid);
		FieldDef* getTemplateFromName(const char* templateName);

		Block* getBlockFromName(const char* name);

	protected:
	private:
		std::vector<FieldDef*> templates;
};

} // namespace DIFK

#endif // DIFK_TEMPLATEDEFINITIONS_H
