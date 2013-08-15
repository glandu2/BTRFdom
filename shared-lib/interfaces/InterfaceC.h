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

#ifndef INTERFACEC_H
#define INTERFACEC_H

#include "ExportDLL.h"
#include "MemberType.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* IObject;

//These are also IObjects
typedef void* IBtrfBlock;
typedef void* IBtrfParser;
typedef void* IBtrfRootBlock;
typedef void* ITmlBlock;
typedef void* ITmlFile;

DLLEXPORT_BTRF void DLLCALLCONV deleteObject(IObject *fieldInfo);


///////////////////////////////////////////
// BtrfBlock
///////////////////////////////////////////
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV createBtrfBlock(ITmlBlock *fieldInfo, IBtrfRootBlock *rootBlock);

DLLEXPORT_BTRF void DLLCALLCONV setElementNumberBtrfBlock(IBtrfBlock *self, int num);
DLLEXPORT_BTRF void DLLCALLCONV setTemplateIdBtrfBlock(IBtrfBlock *self, int id);

DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV getFieldInfoBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF const char* DLLCALLCONV getNameBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF int DLLCALLCONV getElementNumberBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF int DLLCALLCONV getTemplateIdBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidBtrfBlock(IBtrfBlock *self);


DLLEXPORT_BTRF void DLLCALLCONV addBlockBtrfBlock(IBtrfBlock *self, IBtrfBlock *other);
DLLEXPORT_BTRF IBtrfBlock* DLLCALLCONV getBlockBtrfBlock(IBtrfBlock *self, int index);

DLLEXPORT_BTRF void DLLCALLCONV setDataCharBtrfBlock(int index, char data);
DLLEXPORT_BTRF void DLLCALLCONV setDataShortBtrfBlock(int index, short data);
DLLEXPORT_BTRF void DLLCALLCONV setDataIntBtrfBlock(int index, int data);
DLLEXPORT_BTRF void DLLCALLCONV setDataFloatBtrfBlock(int index, float data);
DLLEXPORT_BTRF void DLLCALLCONV setDataStringIdBtrfBlock(int index, int id);

DLLEXPORT_BTRF void DLLCALLCONV setDataCharPtrBtrfBlock(IBtrfBlock *self, char *data);
DLLEXPORT_BTRF void DLLCALLCONV setDataShortPtrBtrfBlock(IBtrfBlock *self, short *data);
DLLEXPORT_BTRF void DLLCALLCONV setDataIntPtrBtrfBlock(IBtrfBlock *self, int *data);
DLLEXPORT_BTRF void DLLCALLCONV setDataFloatPtrBtrfBlock(IBtrfBlock *self, float *data);
DLLEXPORT_BTRF void DLLCALLCONV setDataStringIdPtrBtrfBlock(IBtrfBlock *self, int *id);

DLLEXPORT_BTRF char DLLCALLCONV getDataCharBtrfBlock(IBtrfBlock *self, int index);
DLLEXPORT_BTRF short DLLCALLCONV getDataShortBtrfBlock(IBtrfBlock *self, int index);
DLLEXPORT_BTRF int DLLCALLCONV getDataIntBtrfBlock(IBtrfBlock *self, int index);
DLLEXPORT_BTRF float DLLCALLCONV getDataFloatBtrfBlock(IBtrfBlock *self, int index);
DLLEXPORT_BTRF const char* DLLCALLCONV getDataStringBtrfBlock(IBtrfBlock *self, int index);
DLLEXPORT_BTRF int DLLCALLCONV getDataStringIdBtrfBlock(IBtrfBlock *self, int index);

DLLEXPORT_BTRF char * DLLCALLCONV getDataCharPtrBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF short * DLLCALLCONV getDataShortPtrBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF int * DLLCALLCONV getDataIntPtrBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF float * DLLCALLCONV getDataFloatPtrBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF int * DLLCALLCONV getDataStringIdPtrBtrfBlock(IBtrfBlock *self);

///////////////////////////////////////////
// BtrfParser
///////////////////////////////////////////
DLLEXPORT_BTRF IBtrfParser * DLLCALLCONV createBtrfParser(ITmlFile *tmlFile);

DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV readFileBtrfParser(IBtrfParser *self, const char* filename);
DLLEXPORT_BTRF void DLLCALLCONV writeFileBtrfParser(IBtrfParser *self, const char* filename, IBtrfRootBlock *rootBlock);

///////////////////////////////////////////
// BtrfRootBlock
///////////////////////////////////////////
DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV createBtrfRootBlock(ITmlFile *tmlFile);

DLLEXPORT_BTRF int DLLCALLCONV addStringBtrfRootBlock(IBtrfRootBlock *self, const char* str);
DLLEXPORT_BTRF const char * DLLCALLCONV getStringBtrfRootBlock(IBtrfRootBlock *self, int index);

DLLEXPORT_BTRF int DLLCALLCONV addTemplateBtrfRootBlock(IBtrfRootBlock *self, const TemplateGuid* guid, int usedField);
DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidBtrfRootBlock(IBtrfRootBlock *self, int index);
DLLEXPORT_BTRF int DLLCALLCONV getTemplateUsedFieldBtrfRootBlock(IBtrfRootBlock *self, int index);
DLLEXPORT_BTRF int DLLCALLCONV getStringNumBtrfRootBlock(IBtrfRootBlock *self);
DLLEXPORT_BTRF int DLLCALLCONV getTemplateNumBtrfRootBlock(IBtrfRootBlock *self);

DLLEXPORT_BTRF ITmlFile * DLLCALLCONV getTmlFileBtrfRootBlock(IBtrfRootBlock *self);

DLLEXPORT_BTRF int DLLCALLCONV addBlockBtrfRootBlock(IBtrfRootBlock *self, IBtrfBlock *block);
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByGuidBtrfRootBlock(IBtrfRootBlock *self, const TemplateGuid* guid);
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByNameBtrfRootBlock(IBtrfRootBlock *self, int id);
DLLEXPORT_BTRF int DLLCALLCONV getBlockNumBtrfRootBlock(IBtrfRootBlock *self);

DLLEXPORT_BTRF void DLLCALLCONV dumpToStdoutBtrfRootBlock(IBtrfRootBlock *self);

///////////////////////////////////////////
// TmlBlock
///////////////////////////////////////////
DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV createTmlBlock();

DLLEXPORT_BTRF const TemplateGuid* DLLCALLCONV getTemplateGuidTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF const char* DLLCALLCONV getNameTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getFieldTmlBlock(ITmlBlock *self, int i);
DLLEXPORT_BTRF int DLLCALLCONV getFieldCountTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF void DLLCALLCONV setFieldCountTmlBlock(ITmlBlock *self, int num);
DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF bool DLLCALLCONV getHasVariableSizeTmlBlock(ITmlBlock *self);

///////////////////////////////////////////
// TmlFile
///////////////////////////////////////////
DLLEXPORT_BTRF ITmlFile * DLLCALLCONV createTmlFile();

DLLEXPORT_BTRF bool DLLCALLCONV parseFileTmlFile(ITmlFile *self, const char *file);
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByGuidTmlFile(ITmlFile *self, const TemplateGuid* guid);
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByNameTmlFile(ITmlFile *self, const char* name);

DLLEXPORT_BTRF void DLLCALLCONV addTemplateTmlFile(ITmlFile *self, ITmlBlock *templateStruct);

#ifdef __cplusplus
}
#endif

#endif // INTERFACEC_H
