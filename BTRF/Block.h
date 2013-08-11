#ifndef DIFK_BLOCK_H
#define DIFK_BLOCK_H

#include <stdio.h>
#include "../TML/MemberType.h"
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

namespace TML {
class Block;
}

namespace BTRF {

class Block
{
public:
	Block() : fieldInfo(nullptr), numElement(0), data(nullptr), allocatedData(false) {}

	void setFieldInfo(TML::Block *fieldInfo) { this->fieldInfo = fieldInfo; }
	void setElementNumber(int num) { numElement = num; }
	void setTemplateGuid(TemplateGuid guid) { templateGuid = guid; }

	TML::Block *getFieldInfo() { return fieldInfo; }
	int getElementNumber() { return numElement; }
	TemplateGuid getTemplateGuid() { return templateGuid; }

	//Copy memory
	void setData(ElementType dataType, int num = 0) { setData(dataType, nullptr, num); }
	void setData(ElementType dataType, void *data, int num = 0);

	//Use a pointer and don't copy memory (in case of memory mapped file for example)
	void setDataPtr(ElementType dataType, int num = 0) { setData(dataType, nullptr, num); }
	void setDataPtr(ElementType dataType, void *data, int num = 0);
	template<typename T> T getData(int index);
	template<typename T> T getDataPtr();
	Block* getBlock(int index);

	void freeData();

	ElementType getType();
	const char* getName();

	void dumpToStdout();


private:
	TML::Block *fieldInfo;
	int numElement;
	void* data;
	bool allocatedData;
	TemplateGuid templateGuid;
};


template<typename T> T Block::getData(int index) {
	if(index >= numElement) {
		std::cerr << "Index too large for data " << getName() << " index " << index << " / " << numElement << '\n';
		::exit(-2);
	}
	return static_cast<T*>(data)[index];
}

template<typename T> T Block::getDataPtr() {
	return static_cast<T>(data);
}

} // namespace DIFK

#endif // DIFK_BLOCK_H
