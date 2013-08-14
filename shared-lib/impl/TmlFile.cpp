/*
 * BTRFdom - Rappelz BTRF Document Object Model
 * By Glandu2
 * Copyright 2013 Glandu2
 *
 * This file is part of BTRFdom.
 * BTRFdom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BTRFdom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with BTRFdom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TmlFile.h"
#include "TmlBlock.h"
#include <iostream>
#include <stdlib.h>

TmlFile::TmlFile()
{
	//ctor
}

TmlFile::~TmlFile() {
	std::unordered_map<TemplateGuid, TmlBlock*>::iterator it;
	std::unordered_map<TemplateGuid, TmlBlock*>::const_iterator itEnd;

	for(it = templatesByGuid.begin(), itEnd = templatesByGuid.cend(); it != itEnd; ++it) {
		delete it->second;
	}

	templatesByGuid.clear();
	templatesByName.clear();
}

TmlBlock* TmlFile::getTemplate(const TemplateGuid& guid) {
	try {
		return templatesByGuid.at(guid);
	} catch(...) {
		std::cerr << "Missing template definition, .tml file is missing ?, guid = " << std::hex << std::uppercase <<
						(unsigned int)guid.Data1 << '-' << (unsigned int)guid.Data2 << '-' << (unsigned int)guid.Data3 << '-' <<
						(unsigned int)guid.Data4[0] << (unsigned int)guid.Data4[1] << '-' << (unsigned int)guid.Data4[2] <<
						(unsigned int)guid.Data4[3] << (unsigned int)guid.Data4[4] << (unsigned int)guid.Data4[5] <<
						(unsigned int)guid.Data4[6] << (unsigned int)guid.Data4[7] << '\n';
		abort();
		return nullptr;
	}
}

TmlBlock* TmlFile::getTemplate(const char *name) {
	try {
		return templatesByName.at(std::string(name));
	} catch(...) {
		std::cerr << "Missing template definition, .tml file is missing ?, field name = " << name << '\n';
		abort();
		return nullptr;
	}
}

bool TmlFile::parseFile(const char *file) {
	TmlBlock *block;
	FILE* tml_file;

	tml_file = fopen(file, "rb");
	if(!tml_file) {
		fprintf(stderr, "Warning: cannot open template file %s\n", file);
		return false;
	}

	while((block = new TmlBlock) != nullptr && block->parseFile(tml_file, this)) {}

	fclose(tml_file);

	return true;
}

void TmlFile::addTemplate(ITmlBlock *iBlock) {
	TmlBlock *block = static_cast<TmlBlock*>(iBlock);
	templatesByGuid.insert(std::pair<TemplateGuid, TmlBlock*>(block->getTemplateGuid(), block));
	templatesByName.insert(std::pair<std::string, TmlBlock*>(block->getName(), block));
}
