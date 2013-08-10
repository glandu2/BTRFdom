#include "TMLFile.h"
#include "Block.h"
#include <iostream>
#include <stdlib.h>

namespace TML {

TMLFile::TMLFile()
{
	//ctor
}

TMLFile::~TMLFile() {
	std::unordered_map<TemplateGuid, Block*>::iterator it;
	std::unordered_map<TemplateGuid, Block*>::const_iterator itEnd;

	for(it = templatesByGuid.begin(), itEnd = templatesByGuid.cend(); it != itEnd; ++it) {
		delete it->second;
	}

	templatesByGuid.clear();
	templatesByName.clear();
}

Block* TMLFile::getTemplate(const TemplateGuid& guid) {
	try {
		return templatesByGuid.at(guid);
	} catch(...) {
		std::cerr << "Missing template definition, .tml file is missing ?, guid = " << std::hex << std::uppercase <<
						(unsigned int)guid.Data1 << '-' << (unsigned int)guid.Data2 << '-' << (unsigned int)guid.Data3 << '-' <<
                        (unsigned int)guid.Data4[0] << (unsigned int)guid.Data4[1] << '-' << (unsigned int)guid.Data4[2] <<
                        (unsigned int)guid.Data4[3] << (unsigned int)guid.Data4[4] << (unsigned int)guid.Data4[5] <<
                        (unsigned int)guid.Data4[6] << (unsigned int)guid.Data4[7] << '\n';
		abort();
	}
}

Block* TMLFile::getTemplate(const std::string& name) {
	try {
		return templatesByName.at(name);
	} catch(...) {
		std::cerr << "Missing template definition, .tml file is missing ?, field name = " << name << '\n';
		abort();
	}
}

void TMLFile::parseFile(FILE *file) {
	Block *block;

	while((block = new Block) != nullptr && block->parseFile(file, this)) {
	}
}

void TMLFile::addTemplate(Block *block) {
	templatesByGuid.insert(std::pair<TemplateGuid, Block*>(block->getTemplateGuid(), block));
	templatesByName.insert(std::pair<std::string, Block*>(block->getName(), block));
}

} // namespace TML
