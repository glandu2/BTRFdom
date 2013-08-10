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
		Block() : numElement(0), data(0) {};

		void* getData() { return data; }
		ElementType getType();
		const char* getName();
		int getNumElement() { return numElement; }
		TemplateGuid getTemplateGuid() { return templateGuid; }

		void dumpToStdout();

		template<typename T> T getData(int index);
		template<typename T> T getDataPtr();
		Block* getBlock(int index);


	protected:
		static ElementType getTypeFromByte(char data);

	private:
		friend class Parser;

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
