#ifndef MEMBERTYPE_H_INCLUDED
#define MEMBERTYPE_H_INCLUDED

#include <string.h>
#include <functional>
#include <windows.h>
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
	ET_TemplateArray = 0x12,
	ET_Array = 0x13


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

	TemplateGuid(GUID guid) {
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
