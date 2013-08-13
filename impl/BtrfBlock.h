#ifndef BTRF_BLOCK_H
#define BTRF_BLOCK_H

#include <stdio.h>
#include "MemberType.h"
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include "IBtrfBlock.h"
#include "TmlBlock.h"

class BtrfRootBlock;

class BtrfBlock : public IBtrfBlock
{
public:
	BtrfBlock() : fieldInfo(nullptr), rootBlock(nullptr), numElement(0), data(nullptr), allocatedData(false), templateId(-1) {}

	void construct(TmlBlock *fieldInfo, BtrfRootBlock *rootBlock) { this->fieldInfo = fieldInfo; this->rootBlock = rootBlock; }

	COM_BEGIN_DECLARE_IFACE
	COM_DECLARE_IFACE(IBtrfBlock)
	COM_END_DECLARE_IFACE

	virtual void DLLCALLCONV setElementNumber(int num) { numElement = num; }
	virtual void DLLCALLCONV setTemplateId(int id) { templateId = id; }

	virtual TmlBlock * DLLCALLCONV getFieldInfo() { return fieldInfo; }
	virtual int DLLCALLCONV getElementNumber() { return numElement; }
	virtual int DLLCALLCONV getTemplateId() { return templateId; }
	virtual TemplateGuid DLLCALLCONV getTemplateGuid();

	//Copy memory
	//String take a array of index to strings (int [])
	virtual void DLLCALLCONV setData(ElementType dataType, int num = 0) { setData(dataType, nullptr, num); }
	virtual void DLLCALLCONV setData(ElementType dataType, void *data, int num = 0);
	virtual void DLLCALLCONV freeData();

	//Use a pointer and don't copy memory (in case of memory mapped file for example)
	virtual void DLLCALLCONV setDataPtr(ElementType dataType, int num = 0) { setData(dataType, nullptr, num); }
	virtual void DLLCALLCONV setDataPtr(ElementType dataType, void *data, int num = 0);

	virtual const void * DLLCALLCONV getData(int index);
	virtual void * DLLCALLCONV getDataPtr() { return data; }

	template<typename T> T getData(int index);
	template<typename T> T getDataPtr();

	virtual BtrfBlock* DLLCALLCONV getBlock(int index);
	virtual char DLLCALLCONV getDataChar(int index) { return getData<char>(index); }
	virtual short DLLCALLCONV getDataShort(int index) { return getData<short>(index); }
	virtual int DLLCALLCONV getDataInt(int index) { return getData<int>(index); }
	virtual float DLLCALLCONV getDataFloat(int index) { return getData<float>(index); }
	inline virtual const char* DLLCALLCONV getDataString(int index);
	virtual int DLLCALLCONV getDataStringId(int index) { return getData<int>(index); }

	virtual char * DLLCALLCONV getDataCharPtr() { return getDataPtr<char*>(); }
	virtual short * DLLCALLCONV getDataShortPtr() { return getDataPtr<short*>(); }
	virtual int * DLLCALLCONV getDataIntPtr() { return getDataPtr<int*>(); }
	virtual float * DLLCALLCONV getDataFloatPtr() { return getDataPtr<float*>(); }
	virtual int * DLLCALLCONV getDataStringIdPtr() { return getDataPtr<int*>(); }


	virtual ElementType DLLCALLCONV getType();
	virtual const char* DLLCALLCONV getName();

	void dumpToStdout();


private:
	TmlBlock *fieldInfo;
	BtrfRootBlock *rootBlock;
	int numElement;
	void* data;
	bool allocatedData;
	int templateId;
};


template<typename T> T BtrfBlock::getData(int index) {
	if(index >= numElement) {
		std::cerr << "Index too large for data " << getName() << " index " << index << " / " << numElement << '\n';
		::exit(-2);
	}
	return static_cast<T*>(data)[index];
}

template<>
const char * BtrfBlock::getData<const char*>(int index);

template<typename T> T BtrfBlock::getDataPtr() {
	return static_cast<T>(data);
}

const char* DLLCALLCONV BtrfBlock::getDataString(int index) { return getData<const char*>(index); }

#endif // DIFK_BLOCK_H
