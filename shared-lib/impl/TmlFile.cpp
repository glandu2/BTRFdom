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
#include <fstream>
#include <sstream>

#include "definitions/nx3.h"
#include "definitions/nobj.h"

TmlFile::TmlFile()
{
	//ctor
}

TmlFile::~TmlFile() {
	std::unordered_map<TemplateGuid, TmlBlock*>::iterator it;

	for(it = templatesByGuid.begin(); it != templatesByGuid.end(); ++it) {
		delete it->second;
	}

	templatesByGuid.clear();
	templatesByName.clear();
}

TmlBlock* TmlFile::getTemplateByGuid(const TemplateGuid& guid) {
	try {
		return templatesByGuid.at(guid);
	} catch(...) {
		std::cerr << "Missing template definition, .tml file is missing ?, guid = " << std::hex << std::uppercase <<
						(unsigned int)guid.Data1 << '-' << (unsigned int)guid.Data2 << '-' << (unsigned int)guid.Data3 << '-' <<
						(unsigned int)guid.Data4[0] << (unsigned int)guid.Data4[1] << '-' << (unsigned int)guid.Data4[2] <<
						(unsigned int)guid.Data4[3] << (unsigned int)guid.Data4[4] << (unsigned int)guid.Data4[5] <<
						(unsigned int)guid.Data4[6] << (unsigned int)guid.Data4[7] << '\n';
		return nullptr;
	}
}

TmlBlock* TmlFile::getTemplateByName(const char *name) {
	try {
		return templatesByName.at(std::string(name));
	} catch(...) {
		std::cerr << "Missing template definition, .tml file is missing ?, field name = " << name << '\n';
		return nullptr;
	}
}

bool TmlFile::parseFile(const char *file) {
	std::ifstream tml_file;

	tml_file.open(file, std::ifstream::in | std::ifstream::binary);

	if(!tml_file.good()) {
		fprintf(stderr, "Warning: cannot open template file %s\n", file);
		return false;
	}

	parseFile(&tml_file);

	tml_file.close();

	return true;
}

void TmlFile::parseFile(std::istream* data) {
	bool ok;
	TmlBlock* block;

	do {
		block = new TmlBlock;
		ok = block->parseFile(data, this);
	} while(ok);

	delete block;
}

void TmlFile::addTemplate(ITmlBlock *iBlock) {
	TmlBlock *block = static_cast<TmlBlock*>(iBlock);
	templatesByGuid.insert(std::pair<TemplateGuid, TmlBlock*>(block->getTemplateGuid(), block));
	templatesByName.insert(std::pair<std::string, TmlBlock*>(block->getName(), block));
}

void TmlFile::loadNx3() {
	std::istringstream data(nx3_tml_data);

	parseFile(&data);
}

void TmlFile::loadNobj() {
	std::istringstream data(nobj_tml_data);

	parseFile(&data);
}
