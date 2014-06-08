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

#ifndef MEMBERTYPE_H_INCLUDED
#define MEMBERTYPE_H_INCLUDED

#include <string.h>
#include <functional>

#include "Guid.h"

/*
enum ElementType {
	ET_None = 0x0,
	ET_Char = 0x1,
	ET_Word = 0x2,
	ET_DWord = 0x3,
	ET_QWord = 0x4,
	ET_Float = 0x5,
	ET_Double = 0x6,
	ET_String = 0x7,
	ET_WString = 0x8,
	ET_Template = 0x9,
	ET_Dict = 0xA,
};*/

enum ElementType {
	/*ET_None = 0x0,
	ET_Word = 0x28,
	ET_DWord = 0x29,
	ET_Float = 0x2A,
	ET_Char = 0x2C,
	ET_UChar = 0x2D,
	ET_String = 0x33,
	ET_Array = 0x34,
	ET_Template = 0x1F,
	ET_TemplateArray = 0x3F*/
	ET_None = 0x0,
	ET_Char = 0x1,
	ET_UChar = 0x11,
	ET_Word = 0x2,
	ET_DWord = 0x3,
	ET_Float = 0x5,
	ET_String = 0x7,
	ET_Template = 0x9,
	ET_Dict = 0x0A,
	//ET_TemplateArray = 0x12,
	ET_Array = 0x13,

	//_ARRAYMASK = 0x80,
	//ET_CharArray = 0x81,
	//ET_WordArray = 0x82,
	//ET_DWordArray = 0x83,
	//LLONG_ARRAY = 0x84,
	//ET_FloatArray = 0x85,
	//ET_DoubleArray = 0x86,
	//ET_StringArray = 0x87,
	//ET_WStringArray = 0x88,
	ET_TemplateArray = 0x89,


	/*ET_QWord = 0x4,
	ET_Double = 0x6,
	ET_WString = 0x8,
	ET_Dict = 0xA*/
};

union TemplateGuid {
	char rawData[16];
	struct {
		unsigned int   Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	};

	TemplateGuid() {
		int i;
		for(i=0; i<16; i++)
			rawData[i] = 0;
	}

	TemplateGuid(const Guid& guid) {
		int i;
		Data1 = guid.Data1;
		Data2 = guid.Data2;
		Data3 = guid.Data3;

		for(i=0; i<8; i++)
			Data4[i] = guid.Data4[i];
	}

	bool operator==(const TemplateGuid& other) const {
		return !memcmp(this, &other, sizeof(TemplateGuid));
	}
};

namespace std {

template <>
class hash<TemplateGuid> {
 public:
  size_t operator()(const TemplateGuid &guid) const
  {
    // computes the hash of an employee using a variant
    // of the Fowler-Noll-Vo hash function
    size_t result = 2166136261u;
    const char* data = (const char*)&guid;

    for (size_t i = 0; i < sizeof(TemplateGuid); ++i) {
      result = (result * 16777619) ^ data[i];
    }

    return result;
  }
};

}

#endif // MEMBERTYPE_H_INCLUDED
