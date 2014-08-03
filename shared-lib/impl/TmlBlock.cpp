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

#include "TmlBlock.h"
#include "TmlFile.h"
#include <stdlib.h>
//#include <string>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <iostream>
#include <sstream>

TmlBlock::TmlBlock()
{
	memset(&guid, 0, sizeof(guid));
	elementType = ET_None;
	numElement = 0;
	hasVariableSize = false;
}

TmlBlock::~TmlBlock() {
	std::deque<TmlBlock*>::iterator it;

	for(it = subfields.begin(); it != subfields.end();) {
		TmlBlock *subblock = *it;
		if(subblock->getType() != ET_TemplateArray)
			delete subblock;
		it = subfields.erase(it);
	}
}

unsigned char strtochar(const char str[2]) {
	unsigned char result;

	if(isdigit(str[0]))
		result = (str[0] - '0') << 4;
	else result = (tolower(str[0]) - 'a' + 0xA) << 4;

	if(isdigit(str[1]))
		result |= (str[1] - '0');
	else result |= (tolower(str[1]) - 'a' + 0xA);

	return result;
}

void DLLCALLCONV TmlBlock::setFieldCount(int num) {
//	if(numElement && num > numElement) {
//		std::cerr << "Error: attempt to set field count to a greater value (" << num << ") than the original (" << numElement << ")! Template's used field number is likely to be wrong\n";
//	} else {
		numElement = num;
//	}
}

bool TmlBlock::parseFile(std::istream* file, TmlFile *tmlFile) {
	char line[1024];
	enum State {
		S_TemplateName,
		S_TemplateGuid,
		S_Begin,
		S_Fields,
		S_End
	} state;

	state = S_TemplateName;

	do {
		if(file->eof())
			return false;
		file->getline(line, 1023, '\n');
		if(line[0] == '/' && line[1] == '/')
			continue;
		switch(state) {
		case S_TemplateName:
			if(!strncmp(line, "template", 7)) {
				char *p = strpbrk(line, "\r\n");
				if(p) *p = 0;
				name = std::string(line+9);
				elementType = ET_Template;
				numElement = 0;
				hasVariableSize = false;
				state = S_TemplateGuid;
			}
			break;

		case S_TemplateGuid:
			if(line[0] == '{' && isalnum(line[1])) {
				guid.Data1 = strtoul(line + 1, NULL, 16);
				guid.Data2 = (unsigned short) strtoul(line + 10, NULL, 16);
				guid.Data3 = (unsigned short) strtoul(line + 15, NULL, 16);
				guid.Data4[0] = strtochar(line + 20);
				guid.Data4[1] = strtochar(line + 22);
				guid.Data4[2] = strtochar(line + 25);
				guid.Data4[3] = strtochar(line + 27);
				guid.Data4[4] = strtochar(line + 29);
				guid.Data4[5] = strtochar(line + 31);
				guid.Data4[6] = strtochar(line + 33);
				guid.Data4[7] = strtochar(line + 35);
				line[36] = 0;
				state = S_Begin;
				/*printf("Template guid: %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
					(unsigned int)guid.Data1, (unsigned int)guid.Data2, (unsigned int)guid.Data3,
					(unsigned int)guid.Data4[0], (unsigned int)guid.Data4[1], (unsigned int)guid.Data4[2],
					(unsigned int)guid.Data4[3], (unsigned int)guid.Data4[4], (unsigned int)guid.Data4[5],
					(unsigned int)guid.Data4[6], (unsigned int)guid.Data4[7]);*/
			}
			break;

		case S_Begin:
			if(line[0] == '{') {
				tmlFile->addTemplate(this);
				state = S_Fields;
			}
			break;

		case S_Fields:
			if(line[0] == '}') {
				state = S_End;
			} else {
				TmlBlock *subBlock;
				char *p1, *p2;

				p1 = line;
				while(*p1 && *p1 != '/' && isspace(*p1)) p1++;
				p2 = p1;
				while(*p2 && *p2 != '/' && !isspace(*p2)) p2++;
				if(!*p2 || p2 == p1)
					continue;
				*p2 = 0;

				subBlock = new TmlBlock();

				if(!strcmp(p1, "char")) {
					subBlock->elementType = ET_Char;
				} else if(!strcmp(p1, "word")) {
					subBlock->elementType = ET_Word;
				} else if(!strcmp(p1, "dword")) {
					subBlock->elementType = ET_DWord;
				} else if(!strcmp(p1, "float")) {
					subBlock->elementType = ET_Float;
				} else if(!strcmp(p1, "string")) {
					subBlock->elementType = ET_String;
				} else if(!strcmp(p1, "dict")) {
					subBlock->elementType = ET_Dict;
					subBlock->hasVariableSize = true;
					hasVariableSize = true;
					subfields.push_back(subBlock);
					numElement++;
					continue;
				} else {
					subBlock->elementType = ET_TemplateArray;
					subBlock->subfields.push_back(tmlFile->getTemplateByName(p1));
				}

				p1 = p2+1;
				while(*p1 && isspace(*p1)) p1++;
				p2 = p1;
				while(*p2 && !isspace(*p2) && *p2 != '[' && *p2 != ';') p2++;
				if(!*p2) {
					delete subBlock;
					continue;
				}

				bool wasCrochet = false;
				if(*p2 == '[')
					wasCrochet = true;
				*p2 = 0;

				subBlock->name = std::string(p1);
				if(wasCrochet)
					p1 = p2;
				else p1 = strchr(p2+1, '[');
				if(p1) {
					p1++;
					p2 = strpbrk(p1, "];\n");
				}

				if(p1 && p2 && *p2 == ']') {
					char *p3;
					*p2 = 0;

					subBlock->numElement = strtol(p1, &p3, 10);
					if(subBlock->numElement == 0 || p3 != p2) {
						std::deque<TmlBlock*>::iterator it;
						std::deque<TmlBlock*>::const_iterator itEnd = subfields.cend();

						subBlock->numElement = 0;
						subBlock->hasVariableSize = true;
						hasVariableSize = true;

						//Delete the referenced field if still in field list
						for(it = subfields.begin(); it != itEnd; ++it) {
							if(!strcmp((*it)->name.c_str(), p1)) {
								delete *it;
								subfields.erase(it);
								numElement--;
								break;
							}
						}
					} else
						subBlock->hasVariableSize = false;
				} else {
					subBlock->numElement = 1;
					subBlock->hasVariableSize = false;
				}

				subfields.push_back(subBlock);
				numElement++;
			}
			break;

		case S_End:
			break;
		}
	} while(state != S_End);

	return true;
}
