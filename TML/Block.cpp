#include "Block.h"
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

namespace TML {

Block::Block()
{
	isValid = false;
	memset(&guid, 0, sizeof(guid));
	elementType = ET_None;
	numElement = 0;
	hasVariableSize = false;
}

Block::~Block() {
	std::deque<Block*>::iterator it;
	std::deque<Block*>::const_iterator itEnd = subfields.cend();

	for(it = subfields.begin(); it != itEnd;) {
		delete *it;
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

bool Block::parseFile(FILE *file, TMLFile *tmlFile) {
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
		if(feof(file))
			return false;
		fgets(line, 1023, file);
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
				guid.Data1 = strtoll(line + 1, NULL, 16);
				guid.Data2 = strtol(line + 10, NULL, 16);
				guid.Data3 = strtol(line + 15, NULL, 16);
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
				Block *subBlock = new Block();
				char *p1, *p2;

				p1 = line;
				while(*p1 && *p1 != '/' && isspace(*p1)) p1++;
				p2 = p1;
				while(*p2 && *p2 != '/' && !isspace(*p2)) p2++;
				if(!*p2 || p2 == p1)
					continue;
				*p2 = 0;

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
				} else {
					subBlock->elementType = ET_TemplateArray;
					subBlock->subfields.push_back(tmlFile->getTemplate(std::string(p1)));
				}

				p1 = p2+1;
				while(*p1 && isspace(*p1)) p1++;
				p2 = p1;
				while(*p2 && !isspace(*p2) && *p2 != '[' && *p2 != ';') p2++;
				if(!*p2)
					continue;

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
						std::deque<Block*>::iterator it;
						std::deque<Block*>::const_iterator itEnd = subfields.cend();

						subBlock->numElement = 0;
						hasVariableSize = true;

						//Delete the referenced field if still in field list
						for(it = subfields.begin(); it != itEnd; ++it) {
							if(!strcmp((*it)->name.c_str(), p1)) {
								subfields.erase(it);
								break;
							}
						}
					}
				} else {
					subBlock->numElement = 1;
				}

				subfields.push_back(subBlock);
				numElement++;
			}
			break;

		case S_End:
			break;
		}
	} while(state != S_End);

	isValid = true;

	return true;
}

} // namespace TML
