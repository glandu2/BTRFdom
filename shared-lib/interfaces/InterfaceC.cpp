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

#include "IBtrfBlock.h"
#include "IBtrfParser.h"
#include "IBtrfRootBlock.h"
#include "ITmlBlock.h"
#include "ITmlFile.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#pragma comment(linker, "/export:setElementNumberBtrfBlock=_setElementNumberBtrfBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setElementNumberBtrfBlock(IBtrfBlock *self, int num) {
	self->setElementNumber(num);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:setTemplateIdBtrfBlock=_setTemplateIdBtrfBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setTemplateIdBtrfBlock(IBtrfBlock *self, int id) {
	self->setTemplateId(id);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getFieldInfoBtrfBlock=_getFieldInfoBtrfBlock@4")
#endif
DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV getFieldInfoBtrfBlock(IBtrfBlock *self) {
	return self->getFieldInfo();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getElementNumberBtrfBlock=_getElementNumberBtrfBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getElementNumberBtrfBlock(IBtrfBlock *self) {
	return self->getElementNumber();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateIdBtrfBlock=_getTemplateIdBtrfBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getTemplateIdBtrfBlock(IBtrfBlock *self) {
	return self->getTemplateId();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidBtrfBlock(IBtrfBlock *self) {
	return &self->getTemplateGuid();
}

//Copy memory
//String take a array of index to strings (int [])
//set num = 0 to use Block's numElement value
#ifdef _MSC_VER
#pragma comment(linker, "/export:setDataBtrfBlock=_setDataBtrfBlock@12")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataBtrfBlock(IBtrfBlock *self, ElementType dataType, int num) {
	self->setData(dataType, num);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:setDataContentBtrfBlock=_setDataContentBtrfBlock@16")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataContentBtrfBlock(IBtrfBlock *self, ElementType dataType, void *data, int num) {
	self->setData(dataType, data, num);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:freeDataBtrfBlock=_freeDataBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV freeDataBtrfBlock(IBtrfBlock *self) {
	self->freeData();
}

//Use a pointer and don't copy memory (in case of memory mapped file for example)
//set num = 0 to use Block's numElement value
#ifdef _MSC_VER
#pragma comment(linker, "/export:setDataPtrBtrfBlock=_setDataPtrBtrfBlock@16")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataPtrBtrfBlock(IBtrfBlock *self, ElementType dataType, void *data, int num) {
	self->setDataPtr(dataType, data, num);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataBtrfBlock=_getDataBtrfBlock@8")
#endif
DLLEXPORT_BTRF const void * DLLCALLCONV getDataBtrfBlock(IBtrfBlock *self, int index) {
	return self->getData(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataPtrBtrfBlock=_getDataPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF void * DLLCALLCONV getDataPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataPtr();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getBlockBtrfBlock=_getBlockBtrfBlock@8")
#endif
DLLEXPORT_BTRF IBtrfBlock* DLLCALLCONV getBlockBtrfBlock(IBtrfBlock *self, int index) {
	return self->getBlock(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataCharBtrfBlock=_getDataCharBtrfBlock@8")
#endif
DLLEXPORT_BTRF char DLLCALLCONV getDataCharBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataChar(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataShortBtrfBlock=_getDataShortBtrfBlock@8")
#endif
DLLEXPORT_BTRF short DLLCALLCONV getDataShortBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataShort(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataIntBtrfBlock=_getDataIntBtrfBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getDataIntBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataInt(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataFloatBtrfBlock=_getDataFloatBtrfBlock@8")
#endif
DLLEXPORT_BTRF float DLLCALLCONV getDataFloatBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataFloat(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataStringBtrfBlock=_getDataStringBtrfBlock@8")
#endif
DLLEXPORT_BTRF const char* DLLCALLCONV getDataStringBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataString(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataStringIdBtrfBlock=_getDataStringIdBtrfBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getDataStringIdBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataStringId(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataCharPtrBtrfBlock=_getDataCharPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF char * DLLCALLCONV getDataCharPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataCharPtr();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataShortPtrBtrfBlock=_getDataShortPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF short * DLLCALLCONV getDataShortPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataShortPtr();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataIntPtrBtrfBlock=_getDataIntPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF int * DLLCALLCONV getDataIntPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataIntPtr();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataFloatPtrBtrfBlock=_getDataFloatPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF float * DLLCALLCONV getDataFloatPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataFloatPtr();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getDataStringIdPtrBtrfBlock=_getDataStringIdPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF int * DLLCALLCONV getDataStringIdPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataStringIdPtr();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTypeBtrfBlock=_getTypeBtrfBlock@4")
#endif
DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeBtrfBlock(IBtrfBlock *self) {
	return self->getType();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getNameBtrfBlock=_getNameBtrfBlock@4")
#endif
DLLEXPORT_BTRF const char* DLLCALLCONV getNameBtrfBlock(IBtrfBlock *self) {
	return self->getName();
}

///////////////////////////////////////////
#ifdef _MSC_VER
#pragma comment(linker, "/export:readFileBtrfParser=_readFileBtrfParser@8")
#endif
DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV readFileBtrfParser(IBtrfParser *self, const char* filename) {
	return self->readFile(filename);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:writeFileBtrfParser=_writeFileBtrfParser@12")
#endif
DLLEXPORT_BTRF void DLLCALLCONV writeFileBtrfParser(IBtrfParser *self, const char* filename, IBtrfRootBlock *rootBlock) {
	self->writeFile(filename, rootBlock);
}

///////////////////////////////////////////
#ifdef _MSC_VER
#pragma comment(linker, "/export:addStringBtrfRootBlock=_addStringBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV addStringBtrfRootBlock(IBtrfRootBlock *self, const char* str) {
	self->addString(str);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getStringBtrfRootBlock=_getStringBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF const char * DLLCALLCONV getStringBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return self->getString(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:addTemplateBtrfRootBlock=_addTemplateBtrfRootBlock@12")
#endif
DLLEXPORT_BTRF void DLLCALLCONV addTemplateBtrfRootBlock(IBtrfRootBlock *self, const TemplateGuid* guid, int usedField) {
	self->addTemplate(*guid, usedField);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateGuidBtrfRootBlock=_getTemplateGuidBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return &self->getTemplateGuid(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateUsedFieldBtrfRootBlock=_getTemplateUsedFieldBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getTemplateUsedFieldBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return self->getTemplateUsedField(index);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getStringNumBtrfRootBlock=_getStringNumBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getStringNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getStringNum();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateNumBtrfRootBlock=_getTemplateNumBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getTemplateNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getTemplateNum();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTmlFileBtrfRootBlock=_getTmlFileBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF ITmlFile * DLLCALLCONV getTmlFileBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getTmlFile();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:addBlockBtrfRootBlock=_addBlockBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV addBlockBtrfRootBlock(IBtrfRootBlock *self, IBtrfBlock *block) {
	self->addBlock(block);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getBlockByGuidBtrfRootBlock=_getBlockByGuidBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByGuidBtrfRootBlock(IBtrfRootBlock *self, const TemplateGuid* guid) {
	return self->getBlock(*guid);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getBlockByNameBtrfRootBlock=_getBlockByNameBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByNameBtrfRootBlock(IBtrfRootBlock *self, int id) {
	return self->getBlock(id);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getBlockNumBtrfRootBlock=_getBlockNumBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getBlockNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getBlockNum();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:dumpToStdoutBtrfRootBlock=_dumpToStdoutBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV dumpToStdoutBtrfRootBlock(IBtrfRootBlock *self) {
	self->dumpToStdout();
}

///////////////////////////////////////////
#ifdef _MSC_VER
#pragma comment(linker, "/export:getIsValidTmlBlock=_getIsValidTmlBlock@4")
#endif
DLLEXPORT_BTRF bool DLLCALLCONV getIsValidTmlBlock(ITmlBlock *self) {
	return self->getIsValid();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateGuidTmlBlock=_getTemplateGuidTmlBlock@4")
#endif
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidTmlBlock(ITmlBlock *self) {
	return &self->getTemplateGuid();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getNameTmlBlock=_getNameTmlBlock@4")
#endif
DLLEXPORT_BTRF const char* DLLCALLCONV getNameTmlBlock(ITmlBlock *self) {
	return self->getName();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getFieldTmlBlock=_getFieldTmlBlock@8")
#endif
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getFieldTmlBlock(ITmlBlock *self, int i) {
	return self->getField(i);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getFieldCountTmlBlock=_getFieldCountTmlBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getFieldCountTmlBlock(ITmlBlock *self) {
	return self->getFieldCount();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:setFieldCountTmlBlock=_setFieldCountTmlBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setFieldCountTmlBlock(ITmlBlock *self, int num) {
	self->setFieldCount(num);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTypeTmlBlock=_getTypeTmlBlock@4")
#endif
DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeTmlBlock(ITmlBlock *self) {
	return self->getType();
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getHasVariableSizeTmlBlock=_getHasVariableSizeTmlBlock@4")
#endif
DLLEXPORT_BTRF bool DLLCALLCONV getHasVariableSizeTmlBlock(ITmlBlock *self) {
	return self->getHasVariableSize();
}

///////////////////////////////////////////
#ifdef _MSC_VER
#pragma comment(linker, "/export:parseFileTmlFile=_parseFileTmlFile@8")
#endif
DLLEXPORT_BTRF bool DLLCALLCONV parseFileTmlFile(ITmlFile *self, const char *file) {
	return self->parseFile(file);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateByGuidTmlFile=_getTemplateByGuidTmlFile@8")
#endif
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByGuidTmlFile(ITmlFile *self, const TemplateGuid* guid) {
	return self->getTemplate(*guid);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:getTemplateByNameTmlFile=_getTemplateByNameTmlFile@8")
#endif
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByNameTmlFile(ITmlFile *self, const char* name) {
	return self->getTemplate(name);
}

#ifdef _MSC_VER
#pragma comment(linker, "/export:addTemplateTmlFile=_addTemplateTmlFile@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV addTemplateTmlFile(ITmlFile *self, ITmlBlock *templateStruct) {
	self->addTemplate(templateStruct);
}

#ifdef __cplusplus
}
#endif
