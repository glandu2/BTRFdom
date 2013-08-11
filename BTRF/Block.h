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

class BTRFFile;

class Block
{
public:
	Block() : numElement(0), data(0) {}

	void setFieldInfo(TML::Block *fieldInfo) { this->fieldInfo = fieldInfo; }
	void setElementNumber(int num) { numElement = num; }
	void setData(void *data) { this->data = data; }
	void setTemplateGuid(TemplateGuid guid) { templateGuid = guid; }

	TML::Block *getFieldInfo() { return fieldInfo; }
	int getElementNumber() { return numElement; }
	void* getData() { return data; }
	TemplateGuid getTemplateGuid() { return templateGuid; }

	ElementType getType();
	const char* getName();

	void dumpToStdout();

	template<typename T> T getData(int index);
	template<typename T> T getDataPtr();
	Block* getBlock(int index);


protected:
	static ElementType getTypeFromByte(char data);

private:
	TML::Block *fieldInfo;
	int numElement;
	void* data;
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
