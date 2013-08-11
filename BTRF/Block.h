#ifndef DIFK_BLOCK_H
#define DIFK_BLOCK_H

#include <stdio.h>
#include "../TML/MemberType.h"
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include "RootBlock.h"

namespace TML {
class Block;
}

namespace BTRF {

class Block
{
public:
	Block() : fieldInfo(nullptr), rootBlock(nullptr), numElement(0), data(nullptr), allocatedData(false), templateId(-1) {}

	void construct(TML::Block *fieldInfo, RootBlock *rootBlock) { this->fieldInfo = fieldInfo; this->rootBlock = rootBlock; }

	void setElementNumber(int num) { numElement = num; }
	void setTemplateId(int id) { templateId = id; }

	TML::Block *getFieldInfo() { return fieldInfo; }
	int getElementNumber() { return numElement; }
	int getTemplateId() { return templateId; }
	TemplateGuid getTemplateGuid();

	//Copy memory
	//String take a array of index to strings (int [])
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
	RootBlock *rootBlock;
	int numElement;
	void* data;
	bool allocatedData;
	int templateId;
};


template<typename T> T Block::getData(int index) {
	if(index >= numElement) {
		std::cerr << "Index too large for data " << getName() << " index " << index << " / " << numElement << '\n';
		::exit(-2);
	}
	return static_cast<T*>(data)[index];
}

template<>
const char * Block::getData<const char*>(int index);

template<typename T> T Block::getDataPtr() {
	return static_cast<T>(data);
}

} // namespace DIFK

#endif // DIFK_BLOCK_H
