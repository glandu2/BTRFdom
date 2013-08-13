#ifndef IBTRFBLOCK_H
#define IBTRFBLOCK_H

#include "IObject.h"
#include "MemberType.h"

class ITmlBlock;
class IBtrfRootBlock;

// {E109854D-B397-4e02-B76A-E702FBC7B1CF}
static const Guid IID_IBtrfBlock =
{{ 0xe109854d, 0xb397, 0x4e02, { 0xb7, 0x6a, 0xe7, 0x2, 0xfb, 0xc7, 0xb1, 0xcf } }};


class IBtrfBlock : public CImplement<IObject>
{
public:
	virtual void DLLCALLCONV setElementNumber(int num) = 0;
	virtual void DLLCALLCONV setTemplateId(int id) = 0;

	virtual ITmlBlock * DLLCALLCONV getFieldInfo() = 0;
	virtual int DLLCALLCONV getElementNumber() = 0;
	virtual int DLLCALLCONV getTemplateId() = 0;
	virtual TemplateGuid DLLCALLCONV getTemplateGuid() = 0;

	//Copy memory
	//String take a array of index to strings (int [])
	virtual void DLLCALLCONV setData(ElementType dataType, int num = 0) = 0;
	virtual void DLLCALLCONV setData(ElementType dataType, void *data, int num = 0) = 0;
	virtual void DLLCALLCONV freeData() = 0;

	//Use a pointer and don't copy memory (in case of memory mapped file for example)
	virtual void DLLCALLCONV setDataPtr(ElementType dataType, int num = 0) = 0;
	virtual void DLLCALLCONV setDataPtr(ElementType dataType, void *data, int num = 0) = 0;

	virtual const void * DLLCALLCONV getData(int index) = 0;
	virtual void * DLLCALLCONV getDataPtr() = 0;

	virtual IBtrfBlock* DLLCALLCONV getBlock(int index) = 0;
	virtual char DLLCALLCONV getDataChar(int index) = 0;
	virtual short DLLCALLCONV getDataShort(int index) = 0;
	virtual int DLLCALLCONV getDataInt(int index) = 0;
	virtual float DLLCALLCONV getDataFloat(int index) = 0;
	virtual const char* DLLCALLCONV getDataString(int index) = 0;
	virtual int DLLCALLCONV getDataStringId(int index) = 0;

	virtual char * DLLCALLCONV getDataCharPtr() = 0;
	virtual short * DLLCALLCONV getDataShortPtr() = 0;
	virtual int * DLLCALLCONV getDataIntPtr() = 0;
	virtual float * DLLCALLCONV getDataFloatPtr() = 0;
	virtual int * DLLCALLCONV getDataStringIdPtr() = 0;

	virtual ElementType DLLCALLCONV getType() = 0;
	virtual const char* DLLCALLCONV getName() = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV createBtrfBlock(ITmlBlock *fieldInfo, IBtrfRootBlock *rootBlock);

#ifdef __cplusplus
}
#endif

#endif // IBTRFBLOCK_H
