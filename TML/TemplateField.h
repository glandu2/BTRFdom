#ifndef TML_TEMPLATEFIELD_H
#define TML_TEMPLATEFIELD_H

#include "../BTRF/MemberType.h"
#include <string>

namespace TML {

class TemplateStructure;

struct TemplateField
{
		ElementType type;
		TemplateStructure *templateStruct;
		std::string name;
		bool variableCount;
		int count;
		int index;
};

} // namespace TML

#endif // TML_TEMPLATEFIELD_H
