#include "Block.h"
#include <stdlib.h>
#include "../TML/Block.h"

char *strdup (const char *s) {
	char *d = (char*)malloc (strlen (s) + 1);   // Space for length plus nul
	if (d == NULL)
		return NULL;                     // No memory
	strcpy (d,s);                        // Copy the characters
	return d;                            // Return the new string
}

namespace BTRF {

ElementType Block::getTypeFromByte(char data) {
	switch(data) {
	case 0x1: return ET_Char;
	case 0x2: return ET_Word;
	case 0x3: return ET_DWord;
	case 0x5: return ET_Float;
	case 0x7: return ET_String;
	case 0x9: return ET_Template;
	}

	return ET_None;
}

Block* Block::getBlock(int index) {
	if(index >= numElement) {
		std::cerr << "Index too large for member " << getName() << " index " << index << " / " << numElement << '\n';
		exit(-2);
	}
	return static_cast<Block*>(data) + index;
}

ElementType Block::getType() {
	return fieldInfo->getType();
}
const char* Block::getName() {
	return fieldInfo->getName().c_str();
}

void Block::dumpToStdout() {
	int i;

	std::cout << std::resetiosflags((std::ios_base::fmtflags)-1);
	//std::cout << getName() << " Block with " << numElement << " elements: ";

	switch(fieldInfo->getType()) {
	case ET_Char:
		std::cout << "Char " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << (int)static_cast<char*>(data)[i] << ", ";
		break;

	case ET_UChar:
		std::cout << "UChar " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << (unsigned int)static_cast<unsigned char*>(data)[i] << ", ";
		break;

	case ET_Word:
		std::cout << "Word " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << static_cast<short*>(data)[i] << ", ";
		break;

	case ET_DWord:
		std::cout << "DWord " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << static_cast<int*>(data)[i] << ", ";
		break;

	case ET_Float:
		std::cout << "Float " << getName() << "[" << numElement << "] = " << std::setprecision(5);
		for(i=0; i<numElement; i++)
			std::cout << static_cast<float*>(data)[i] << ", ";
		break;

	case ET_String:
		std::cout << "String " << getName() << "[" << numElement << "] = \n{\n";
		for(i=0; i<numElement; i++) {
			if(static_cast<char**>(data)[i])
				std::cout << static_cast<char**>(data)[i] << '\n';
			else std::cout << "<NULL>\n";
		}
		std::cout << "}";
		break;

	case ET_Template:
		std::cout << "Template GUID = " << std::hex << std::uppercase <<
					 (unsigned int)templateGuid.Data1 << '-' << (unsigned int)templateGuid.Data2 << '-' << (unsigned int)templateGuid.Data3 << '-' <<
					 (unsigned int)templateGuid.Data4[0] << (unsigned int)templateGuid.Data4[1] << '-' << (unsigned int)templateGuid.Data4[2] <<
					 (unsigned int)templateGuid.Data4[3] << (unsigned int)templateGuid.Data4[4] << (unsigned int)templateGuid.Data4[5] <<
					 (unsigned int)templateGuid.Data4[6] << (unsigned int)templateGuid.Data4[7] << std::dec << " " << getName() << ", " << numElement << " subfields\n{\n";
		for(i=0; i<numElement; i++)
			static_cast<Block*>(data)[i].dumpToStdout();
		std::cout << "}";
		break;

	case ET_TemplateArray:
		std::cout << "Template array " << getName() << "[" << numElement << "] = \n{\n";
		for(i=0; i<numElement; i++)
			static_cast<Block*>(data)[i].dumpToStdout();
		std::cout << "}";
		break;

	case ET_Array:
	case ET_None:
		std::cerr << "Internal error, magic is None or Array\n";
		exit(-1);
	}
	std::cout << std::resetiosflags((std::ios_base::fmtflags)-1);

	std::cout << '\n';
}

} // namespace DIFK
