#ifndef INTERFACEC_H
#define INTERFACEC_H

#include "ExportDLL.h"
#include "MemberType.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* IBtrfBlock;
typedef void* IBtrfParser;
typedef void* IBtrfRootBlock;
typedef void* ITmlBlock;
typedef void* ITmlFile;

DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV createBtrfBlock(ITmlBlock *fieldInfo, IBtrfRootBlock *rootBlock);
DLLEXPORT_BTRF void DLLCALLCONV setElementNumberBtrfBlock(IBtrfBlock *self, int num);
DLLEXPORT_BTRF void DLLCALLCONV setTemplateIdBtrfBlock(IBtrfBlock *self, int id);

DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV getFieldInfoBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF int DLLCALLCONV getElementNumberBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF int DLLCALLCONV getTemplateIdBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF TemplateGuid DLLCALLCONV getTemplateGuidBtrfBlock(IBtrfBlock *self);

//Copy memory
//String take a array of index to strings (int [])
//set num = 0 to use Block's numElement value
DLLEXPORT_BTRF void DLLCALLCONV setDataBtrfBlock(IBtrfBlock *self, ElementType dataType, int num);
DLLEXPORT_BTRF void DLLCALLCONV setDataContentBtrfBlock(IBtrfBlock *self, ElementType dataType, void *data, int num);
DLLEXPORT_BTRF void DLLCALLCONV freeDataBtrfBlock(IBtrfBlock *self);

//Use a pointer and don't copy memory (in case of memory mapped file for example)
//set num = 0 to use Block's numElement value
DLLEXPORT_BTRF void DLLCALLCONV setDataPtrBtrfBlock(IBtrfBlock *self, ElementType dataType, void *data, int num);

DLLEXPORT_BTRF const void * DLLCALLCONV getDataBtrfBlock(IBtrfBlock *self, int index);
DLLEXPORT_BTRF void * DLLCALLCONV getDataPtrBtrfBlock(IBtrfBlock *self);

DLLEXPORT_BTRF IBtrfBlock* DLLCALLCONV getBlockBtrfBlock(IBtrfBlock *self, int index);
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

DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeBtrfBlock(IBtrfBlock *self);
DLLEXPORT_BTRF const char* DLLCALLCONV getNameBtrfBlock(IBtrfBlock *self);

///////////////////////////////////////////
DLLEXPORT_BTRF IBtrfParser * DLLCALLCONV createBtrfParser(ITmlFile *tmlFile);
DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV readFileBtrfParser(IBtrfParser *self, const char* filename);
DLLEXPORT_BTRF void DLLCALLCONV writeFileBtrfParser(IBtrfParser *self, const char* filename, IBtrfRootBlock *rootBlock);

///////////////////////////////////////////
DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV createBtrfRootBlock(ITmlFile *tmlFile);

DLLEXPORT_BTRF void DLLCALLCONV addStringBtrfRootBlock(IBtrfRootBlock *self, const char* str);
DLLEXPORT_BTRF const char * DLLCALLCONV getStringBtrfRootBlock(IBtrfRootBlock *self, int index);

DLLEXPORT_BTRF void DLLCALLCONV addTemplateBtrfRootBlock(IBtrfRootBlock *self, TemplateGuid guid, int usedField);
DLLEXPORT_BTRF TemplateGuid DLLCALLCONV getTemplateGuidBtrfRootBlock(IBtrfRootBlock *self, int index);
DLLEXPORT_BTRF int DLLCALLCONV getTemplateUsedFieldBtrfRootBlock(IBtrfRootBlock *self, int index);
DLLEXPORT_BTRF int DLLCALLCONV getStringNumBtrfRootBlock(IBtrfRootBlock *self);
DLLEXPORT_BTRF int DLLCALLCONV getTemplateNumBtrfRootBlock(IBtrfRootBlock *self);

DLLEXPORT_BTRF ITmlFile * DLLCALLCONV getTmlFileBtrfRootBlock(IBtrfRootBlock *self);

DLLEXPORT_BTRF void DLLCALLCONV addBlockBtrfRootBlock(IBtrfRootBlock *self, IBtrfBlock *block);
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByGuidBtrfRootBlock(IBtrfRootBlock *self, TemplateGuid guid);
DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV getBlockByNameBtrfRootBlock(IBtrfRootBlock *self, int id);
DLLEXPORT_BTRF int DLLCALLCONV getBlockNumBtrfRootBlock(IBtrfRootBlock *self);

DLLEXPORT_BTRF void DLLCALLCONV dumpToStdoutBtrfRootBlock(IBtrfRootBlock *self);

///////////////////////////////////////////
DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV createTmlBlock();

DLLEXPORT_BTRF bool DLLCALLCONV getIsValidTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF TemplateGuid DLLCALLCONV getTemplateGuidTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF const char* DLLCALLCONV getNameTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getFieldTmlBlock(ITmlBlock *self, int i);
DLLEXPORT_BTRF int DLLCALLCONV getFieldCountTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF void DLLCALLCONV setFieldCountTmlBlock(ITmlBlock *self, int num);
DLLEXPORT_BTRF ElementType DLLCALLCONV getTypeTmlBlock(ITmlBlock *self);
DLLEXPORT_BTRF bool DLLCALLCONV getHasVariableSizeTmlBlock(ITmlBlock *self);

///////////////////////////////////////////
DLLEXPORT_BTRF ITmlFile * DLLCALLCONV createTmlFile();

DLLEXPORT_BTRF bool DLLCALLCONV parseFileTmlFile(ITmlFile *self, const char *file);
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByGuidTmlFile(ITmlFile *self, const TemplateGuid& guid);
DLLEXPORT_BTRF ITmlBlock* DLLCALLCONV getTemplateByNameTmlFile(ITmlFile *self, const char* name);

DLLEXPORT_BTRF void DLLCALLCONV addTemplateTmlFile(ITmlFile *self, ITmlBlock *templateStruct);

#ifdef __cplusplus
}
#endif

#endif // INTERFACEC_H
