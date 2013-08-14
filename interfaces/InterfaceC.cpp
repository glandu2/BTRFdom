#include "IBtrfBlock.h"
#include "IBtrfParser.h"
#include "IBtrfRootBlock.h"
#include "ITmlBlock.h"
#include "ITmlFile.h"


#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF void DLLCALLCONV setElementNumberBtrfBlock(IBtrfBlock *self, int num) {
	self->setElementNumber(num);
}


DLLEXPORT_BTRF void DLLCALLCONV setTemplateIdBtrfBlock(IBtrfBlock *self, int id) {
	self->setTemplateId(id);
}

DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV getFieldInfoBtrfBlock(IBtrfBlock *self) {
	return self->getFieldInfo();
}

DLLEXPORT_BTRF int DLLCALLCONV getElementNumberBtrfBlock(IBtrfBlock *self) {
	return self->getElementNumber();
}

DLLEXPORT_BTRF int DLLCALLCONV getTemplateIdBtrfBlock(IBtrfBlock *self) {
	return self->getTemplateId();
}

DLLEXPORT_BTRF TemplateGuid DLLCALLCONV getTemplateGuidBtrfBlock(IBtrfBlock *self) {
	return self->getTemplateGuid();
}

//Copy memory
//String take a array of index to strings (int [])
//set num = 0 to use Block's numElement value
DLLEXPORT_BTRF void DLLCALLCONV setDataBtrfBlock(IBtrfBlock *self, ElementType dataType, int num) {
	self->setData(dataType, num);
}

DLLEXPORT_BTRF void DLLCALLCONV setDataContentBtrfBlock(IBtrfBlock *self, ElementType dataType, void *data, int num) {
	self->setData(dataType, data, num);
}

DLLEXPORT_BTRF void DLLCALLCONV freeDataBtrfBlock(IBtrfBlock *self) {
	self->freeData();
}

//Use a pointer and don't copy memory (in case of memory mapped file for example)
//set num = 0 to use Block's numElement value
DLLEXPORT_BTRF void DLLCALLCONV setDataPtrBtrfBlock(IBtrfBlock *self, ElementType dataType, void *data, int num) {
	self->setDataPtr(dataType, data, num);
}

DLLEXPORT_BTRF const void * DLLCALLCONV getDataBtrfBlock(IBtrfBlock *self, int index) {
	return self->getData(index);
}

DLLEXPORT_BTRF void * DLLCALLCONV getDataPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataPtr();
}

DLLEXPORT_BTRF IBtrfBlock* DLLCALLCONV getBlockBtrfBlock(IBtrfBlock *self, int index) {
	return self->getBlock(index);
}

DLLEXPORT_BTRF char DLLCALLCONV getDataCharBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataChar(index);
}

DLLEXPORT_BTRF short DLLCALLCONV getDataShortBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataShort(index);
}

DLLEXPORT_BTRF int DLLCALLCONV getDataIntBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataInt(index);
}

DLLEXPORT_BTRF float DLLCALLCONV getDataFloatBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataFloat(index);
}

DLLEXPORT_BTRF const char* DLLCALLCONV getDataStringBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataString(index);
}

DLLEXPORT_BTRF int DLLCALLCONV getDataStringIdBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataStringId(index);
}

DLLEXPORT_BTRF char * DLLCALLCONV getDataCharPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataCharPtr();
}

DLLEXPORT_BTRF short * DLLCALLCONV getDataShortPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataShortPtr();
}

DLLEXPORT_BTRF int * DLLCALLCONV getDataIntPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataIntPtr();
}

DLLEXPORT_BTRF float * DLLCALLCONV getDataFloatPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataFloatPtr();
}

DLLEXPORT_BTRF int * DLLCALLCONV getDataStringIdPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataStringIdPtr();
}

DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeBtrfBlock(IBtrfBlock *self) {
	return self->getType();
}

DLLEXPORT_BTRF const char* DLLCALLCONV getNameBtrfBlock(IBtrfBlock *self) {
	return self->getName();
}

///////////////////////////////////////////
DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV readFileBtrfParser(IBtrfParser *self, const char* filename) {
	return self->readFile(filename);
}

DLLEXPORT_BTRF void DLLCALLCONV writeFileBtrfParser(IBtrfParser *self, const char* filename, IBtrfRootBlock *rootBlock) {
	self->writeFile(filename, rootBlock);
}

///////////////////////////////////////////
DLLEXPORT_BTRF void DLLCALLCONV addStringBtrfRootBlock(IBtrfRootBlock *self, const char* str) {
	self->addString(str);
}

DLLEXPORT_BTRF const char * DLLCALLCONV getStringBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return self->getString(index);
}

DLLEXPORT_BTRF void DLLCALLCONV addTemplateBtrfRootBlock(IBtrfRootBlock *self, TemplateGuid guid, int usedField) {
	self->addTemplate(guid, usedField);
}

DLLEXPORT_BTRF TemplateGuid DLLCALLCONV getTemplateGuidBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return self->getTemplateGuid(index);
}

DLLEXPORT_BTRF int DLLCALLCONV getTemplateUsedFieldBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return self->getTemplateUsedField(index);
}

DLLEXPORT_BTRF int DLLCALLCONV getStringNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getStringNum();
}

DLLEXPORT_BTRF int DLLCALLCONV getTemplateNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getTemplateNum();
}

DLLEXPORT_BTRF ITmlFile * DLLCALLCONV getTmlFileBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getTmlFile();
}

DLLEXPORT_BTRF void DLLCALLCONV addBlockBtrfRootBlock(IBtrfRootBlock *self, IBtrfBlock *block) {
	self->addBlock(block);
}

DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByGuidBtrfRootBlock(IBtrfRootBlock *self, TemplateGuid guid) {
	return self->getBlock(guid);
}

DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByNameBtrfRootBlock(IBtrfRootBlock *self, int id) {
	return self->getBlock(id);
}

DLLEXPORT_BTRF int DLLCALLCONV getBlockNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getBlockNum();
}

DLLEXPORT_BTRF void DLLCALLCONV dumpToStdoutBtrfRootBlock(IBtrfRootBlock *self) {
	self->dumpToStdout();
}

///////////////////////////////////////////
DLLEXPORT_BTRF bool DLLCALLCONV getIsValidTmlBlock(ITmlBlock *self) {
	return self->getIsValid();
}

DLLEXPORT_BTRF TemplateGuid DLLCALLCONV getTemplateGuidTmlBlock(ITmlBlock *self) {
	return self->getTemplateGuid();
}

DLLEXPORT_BTRF const char* DLLCALLCONV getNameTmlBlock(ITmlBlock *self) {
	return self->getName();
}

DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getFieldTmlBlock(ITmlBlock *self, int i) {
	return self->getField(i);
}

DLLEXPORT_BTRF int DLLCALLCONV getFieldCountTmlBlock(ITmlBlock *self) {
	return self->getFieldCount();
}

DLLEXPORT_BTRF void DLLCALLCONV setFieldCountTmlBlock(ITmlBlock *self, int num) {
	self->setFieldCount(num);
}

DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeTmlBlock(ITmlBlock *self) {
	return self->getType();
}

DLLEXPORT_BTRF bool DLLCALLCONV getHasVariableSizeTmlBlock(ITmlBlock *self) {
	return self->getHasVariableSize();
}

///////////////////////////////////////////
DLLEXPORT_BTRF bool DLLCALLCONV parseFileTmlFile(ITmlFile *self, const char *file) {
	return self->parseFile(file);
}

DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByGuidTmlFile(ITmlFile *self, const TemplateGuid& guid) {
	return self->getTemplate(guid);
}

DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByNameTmlFile(ITmlFile *self, const char* name) {
	return self->getTemplate(name);
}

DLLEXPORT_BTRF void DLLCALLCONV addTemplateTmlFile(ITmlFile *self, ITmlBlock *templateStruct) {
	self->addTemplate(templateStruct);
}

#ifdef __cplusplus
}
#endif
