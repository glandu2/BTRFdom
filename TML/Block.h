#ifndef TML_BLOCK_H
#define TML_BLOCK_H

#include "TMLFile.h"
#include "MemberType.h"
#include <deque>
#include <stdio.h>

namespace TML {

class Block
{
	public:
		Block();
		~Block();

		bool parseFile(FILE *file, TMLFile *tmlFile);

		bool getIsValid() { return isValid; }
		const TemplateGuid& getTemplateGuid() { return guid; }
		const std::string& getName() { return name; }
		Block* getField(int i) { return subfields.at(i); }
		int getFieldCount() { return numElement; }
		void setFieldCount(int num) { numElement = num; }
		ElementType getType() { return elementType; }
		bool getHasVariableSize() { return hasVariableSize; }

	protected:
	private:
		bool isValid;
		std::string name;
		TemplateGuid guid;
		std::deque<Block*> subfields;
		ElementType elementType;
		int numElement;
		bool hasVariableSize;
};

} // namespace TML

#endif // TML_BLOCK_H
