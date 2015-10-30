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
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:deleteObject=_deleteObject@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV deleteObject(IObject *fieldInfo) {
	fieldInfo->destroy();
}


///////////////////////////////////////////
// BtrfBlock
///////////////////////////////////////////

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:setElementNumberBtrfBlock=_setElementNumberBtrfBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setElementNumberBtrfBlock(IBtrfBlock *self, int num) {
	self->setElementNumber(num);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:setTemplateIdBtrfBlock=_setTemplateIdBtrfBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setTemplateIdBtrfBlock(IBtrfBlock *self, int id) {
	self->setTemplateId(id);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getFieldInfoBtrfBlock=_getFieldInfoBtrfBlock@4")
#endif
DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV getFieldInfoBtrfBlock(IBtrfBlock *self) {
	return self->getFieldInfo();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTypeBtrfBlock=_getTypeBtrfBlock@4")
#endif
DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeBtrfBlock(IBtrfBlock *self) {
	return self->getType();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getNameBtrfBlock=_getNameBtrfBlock@4")
#endif
DLLEXPORT_BTRF const char* DLLCALLCONV getNameBtrfBlock(IBtrfBlock *self) {
	return self->getName();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getElementNumberBtrfBlock=_getElementNumberBtrfBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getElementNumberBtrfBlock(IBtrfBlock *self) {
	return self->getElementNumber();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateIdBtrfBlock=_getTemplateIdBtrfBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getTemplateIdBtrfBlock(IBtrfBlock *self) {
	return self->getTemplateId();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidBtrfBlock(IBtrfBlock *self) {
	return &self->getTemplateGuid();
}


//addBlock & getBlock

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:addBlockBtrfBlock=_addBlockBtrfBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV addBlockBtrfBlock(IBtrfBlock *self, IBtrfBlock *other) {
	return self->addBlock(other);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getBlockBtrfBlock=_getBlockBtrfBlock@8")
#endif
DLLEXPORT_BTRF IBtrfBlock* DLLCALLCONV getBlockBtrfBlock(IBtrfBlock *self, int index) {
	return self->getBlock(index);
}


//setData*()

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataCharBtrfBlock(IBtrfBlock *self, int index, char data) {
	self->setDataChar(index, data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataShortBtrfBlock(IBtrfBlock *self, int index, short data) {
	self->setDataShort(index, data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataIntBtrfBlock(IBtrfBlock *self, int index, int data) {
	self->setDataInt(index, data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataFloatBtrfBlock(IBtrfBlock *self, int index, float data) {
	self->setDataFloat(index, data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataStringIdBtrfBlock(IBtrfBlock *self, int index, int id) {
	self->setDataStringId(index, id);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:setDataStringBtrfBlock=_setDataStringBtrfBlock@12")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataStringBtrfBlock(IBtrfBlock *self, int index, const char* data) {
	self->setDataString(index, data);
}


//setData*Ptr()

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataCharPtrBtrfBlock(IBtrfBlock *self, char *data) {
	self->setDataCharPtr(data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataShortPtrBtrfBlock(IBtrfBlock *self, short *data) {
	self->setDataShortPtr(data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataIntPtrBtrfBlock(IBtrfBlock *self, int *data) {
	self->setDataIntPtr(data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataFloatPtrBtrfBlock(IBtrfBlock *self, float *data) {
	self->setDataFloatPtr(data);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfBlock=_getTemplateGuidBtrfBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setDataStringIdPtrBtrfBlock(IBtrfBlock *self, int *id) {
	self->setDataStringIdPtr(id);
}


//getData*()

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataCharBtrfBlock=_getDataCharBtrfBlock@8")
#endif
DLLEXPORT_BTRF char DLLCALLCONV getDataCharBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataChar(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataShortBtrfBlock=_getDataShortBtrfBlock@8")
#endif
DLLEXPORT_BTRF short DLLCALLCONV getDataShortBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataShort(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataIntBtrfBlock=_getDataIntBtrfBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getDataIntBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataInt(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataFloatBtrfBlock=_getDataFloatBtrfBlock@8")
#endif
DLLEXPORT_BTRF float DLLCALLCONV getDataFloatBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataFloat(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataStringBtrfBlock=_getDataStringBtrfBlock@8")
#endif
DLLEXPORT_BTRF const char* DLLCALLCONV getDataStringBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataString(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataStringIdBtrfBlock=_getDataStringIdBtrfBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getDataStringIdBtrfBlock(IBtrfBlock *self, int index) {
	return self->getDataStringId(index);
}


//getData*Ptr()

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataCharPtrBtrfBlock=_getDataCharPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF char * DLLCALLCONV getDataCharPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataCharPtr();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataShortPtrBtrfBlock=_getDataShortPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF short * DLLCALLCONV getDataShortPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataShortPtr();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataIntPtrBtrfBlock=_getDataIntPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF int * DLLCALLCONV getDataIntPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataIntPtr();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataFloatPtrBtrfBlock=_getDataFloatPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF float * DLLCALLCONV getDataFloatPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataFloatPtr();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getDataStringIdPtrBtrfBlock=_getDataStringIdPtrBtrfBlock@4")
#endif
DLLEXPORT_BTRF int * DLLCALLCONV getDataStringIdPtrBtrfBlock(IBtrfBlock *self) {
	return self->getDataStringIdPtr();
}

///////////////////////////////////////////
// BtrfParser
///////////////////////////////////////////
#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:readFileBtrfParser=_readFileBtrfParser@8")
#endif
DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV readFileBtrfParser(IBtrfParser *self, const char* filename) {
	return self->readFile(filename);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:writeFileBtrfParser=_writeFileBtrfParser@12")
#endif
DLLEXPORT_BTRF void DLLCALLCONV writeFileBtrfParser(IBtrfParser *self, const char* filename, IBtrfRootBlock *rootBlock) {
	self->writeFile(filename, rootBlock);
}

///////////////////////////////////////////
// BtrfRootBlock
///////////////////////////////////////////
#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:addStringBtrfRootBlock=_addStringBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV addStringBtrfRootBlock(IBtrfRootBlock *self, const char* str) {
	return self->addString(str);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getStringBtrfRootBlock=_getStringBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF const char * DLLCALLCONV getStringBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return self->getString(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:addTemplateBtrfRootBlock=_addTemplateBtrfRootBlock@12")
#endif
DLLEXPORT_BTRF int DLLCALLCONV addTemplateBtrfRootBlock(IBtrfRootBlock *self, const TemplateGuid* guid, int usedField) {
	return self->addTemplate(*guid, usedField);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidBtrfRootBlock=_getTemplateGuidBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return &self->getTemplateGuid(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateUsedFieldBtrfRootBlock=_getTemplateUsedFieldBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getTemplateUsedFieldBtrfRootBlock(IBtrfRootBlock *self, int index) {
	return self->getTemplateUsedField(index);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getStringNumBtrfRootBlock=_getStringNumBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getStringNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getStringNum();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateNumBtrfRootBlock=_getTemplateNumBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getTemplateNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getTemplateNum();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTmlFileBtrfRootBlock=_getTmlFileBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF ITmlFile * DLLCALLCONV getTmlFileBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getTmlFile();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:addBlockBtrfRootBlock=_addBlockBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF int DLLCALLCONV addBlockBtrfRootBlock(IBtrfRootBlock *self, IBtrfBlock *block) {
	return self->addBlock(block);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getBlockByGuidBtrfRootBlock=_getBlockByGuidBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByGuidBtrfRootBlock(IBtrfRootBlock *self, const TemplateGuid* guid) {
	return self->getBlockByGuid(*guid);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getBlockByNameBtrfRootBlock=_getBlockByNameBtrfRootBlock@8")
#endif
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByNameBtrfRootBlock(IBtrfRootBlock *self, int id) {
	return self->getBlockById(id);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getBlockNumBtrfRootBlock=_getBlockNumBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getBlockNumBtrfRootBlock(IBtrfRootBlock *self) {
	return self->getBlockNum();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:dumpToStdoutBtrfRootBlock=_dumpToStdoutBtrfRootBlock@4")
#endif
DLLEXPORT_BTRF void DLLCALLCONV dumpToStdoutBtrfRootBlock(IBtrfRootBlock *self) {
	self->dumpToStdout(stdout);
}

///////////////////////////////////////////
// TmlBlock
///////////////////////////////////////////
#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateGuidTmlBlock=_getTemplateGuidTmlBlock@4")
#endif
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidTmlBlock(ITmlBlock *self) {
	return &self->getTemplateGuid();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getNameTmlBlock=_getNameTmlBlock@4")
#endif
DLLEXPORT_BTRF const char* DLLCALLCONV getNameTmlBlock(ITmlBlock *self) {
	return self->getName();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getFieldTmlBlock=_getFieldTmlBlock@8")
#endif
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getFieldTmlBlock(ITmlBlock *self, int i) {
	return self->getField(i);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getFieldCountTmlBlock=_getFieldCountTmlBlock@4")
#endif
DLLEXPORT_BTRF int DLLCALLCONV getFieldCountTmlBlock(ITmlBlock *self) {
	return self->getFieldCount();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:setFieldCountTmlBlock=_setFieldCountTmlBlock@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV setFieldCountTmlBlock(ITmlBlock *self, int num) {
	self->setFieldCount(num);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTypeTmlBlock=_getTypeTmlBlock@4")
#endif
DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeTmlBlock(ITmlBlock *self) {
	return self->getType();
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getHasVariableSizeTmlBlock=_getHasVariableSizeTmlBlock@4")
#endif
DLLEXPORT_BTRF bool DLLCALLCONV getHasVariableSizeTmlBlock(ITmlBlock *self) {
	return self->getHasVariableSize();
}

///////////////////////////////////////////
// TmlFile
///////////////////////////////////////////
#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:parseFileTmlFile=_parseFileTmlFile@8")
#endif
DLLEXPORT_BTRF bool DLLCALLCONV parseFileTmlFile(ITmlFile *self, const char *file) {
	return self->parseFile(file);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateByGuidTmlFile=_getTemplateByGuidTmlFile@8")
#endif
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByGuidTmlFile(ITmlFile *self, const TemplateGuid* guid) {
	return self->getTemplateByGuid(*guid);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:getTemplateByNameTmlFile=_getTemplateByNameTmlFile@8")
#endif
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByNameTmlFile(ITmlFile *self, const char* name) {
	return self->getTemplateByName(name);
}

#if defined(_MSC_VER) && !defined(_WIN64)
#pragma comment(linker, "/export:addTemplateTmlFile=_addTemplateTmlFile@8")
#endif
DLLEXPORT_BTRF void DLLCALLCONV addTemplateTmlFile(ITmlFile *self, ITmlBlock *templateStruct) {
	self->addTemplate(templateStruct);
}

#ifdef __cplusplus
}
#endif
