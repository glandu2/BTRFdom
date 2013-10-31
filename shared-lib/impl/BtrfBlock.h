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

class BtrfBlock : public CImplement<IBtrfBlock>
{
public:
	BtrfBlock(TmlBlock *fieldInfo, BtrfRootBlock *rootBlock);
	virtual ~BtrfBlock();

	COM_BEGIN_DECLARE_IFACE
	COM_DECLARE_IFACE(IBtrfBlock)
	COM_END_DECLARE_IFACE

	//internal

	void initData();
	void freeData();

	template<typename T> void setData(int index, T data);
	template<typename T> T getData(int index);
	template<typename T> void setDataPtr(T* data);
	template<typename T> T* getDataPtr();
	void dumpToStdout(FILE *fout);

public:
	virtual void DLLCALLCONV setElementNumber(int num);
	virtual void DLLCALLCONV setTemplateId(int id) { templateId = id; }

	virtual TmlBlock * DLLCALLCONV getFieldInfo() { return fieldInfo; }
	virtual ElementType DLLCALLCONV getType() { return fieldInfo->getType(); }
	virtual const char* DLLCALLCONV getName() { return fieldInfo->getName(); }
	virtual int DLLCALLCONV getElementNumber() { return numElement; }
	virtual int DLLCALLCONV getTemplateId() { return templateId; }
	virtual const TemplateGuid& DLLCALLCONV getTemplateGuid();

	virtual int DLLCALLCONV addBlock(IBtrfBlock *block);
	virtual BtrfBlock* DLLCALLCONV getBlock(int index);

	virtual void DLLCALLCONV setDataChar(int index, char data) { setData<char>(index, data); }
	virtual void DLLCALLCONV setDataShort(int index, short data) { setData<short>(index, data); }
	virtual void DLLCALLCONV setDataInt(int index, int data) { setData<int>(index, data); }
	virtual void DLLCALLCONV setDataFloat(int index, float data) { setData<float>(index, data); }
	virtual void DLLCALLCONV setDataStringId(int index, int id) { setData<int>(index, id); }
	virtual void DLLCALLCONV setDataString(int index, const char* data);

	virtual void DLLCALLCONV setDataCharPtr(char *data) { setDataPtr<char>(data); }
	virtual void DLLCALLCONV setDataShortPtr(short *data) { setDataPtr<short>(data); }
	virtual void DLLCALLCONV setDataIntPtr(int *data) { setDataPtr<int>(data); }
	virtual void DLLCALLCONV setDataFloatPtr(float *data) { setDataPtr<float>(data); }
	virtual void DLLCALLCONV setDataStringIdPtr(int *id) { setDataPtr<int>(id); }

	virtual char DLLCALLCONV getDataChar(int index) { return getData<char>(index); }
	virtual short DLLCALLCONV getDataShort(int index) { return getData<short>(index); }
	virtual int DLLCALLCONV getDataInt(int index) { return getData<int>(index); }
	virtual float DLLCALLCONV getDataFloat(int index) { return getData<float>(index); }
	virtual inline const char* DLLCALLCONV getDataString(int index);
	virtual int DLLCALLCONV getDataStringId(int index) { return getData<int>(index); }

	virtual char * DLLCALLCONV getDataCharPtr() { return getDataPtr<char>(); }
	virtual short * DLLCALLCONV getDataShortPtr() { return getDataPtr<short>(); }
	virtual int * DLLCALLCONV getDataIntPtr() { return getDataPtr<int>(); }
	virtual float * DLLCALLCONV getDataFloatPtr() { return getDataPtr<float>(); }
	virtual int * DLLCALLCONV getDataStringIdPtr() { return getDataPtr<int>(); }

protected:
	bool checkIndexType(int type, int index);

private:
	TmlBlock *fieldInfo;
	BtrfRootBlock *rootBlock;
	int numElement;
	void* data;
	bool allocatedData;
	int templateId;
};

template<> const char * BtrfBlock::getData<const char*>(int index);

const char* DLLCALLCONV BtrfBlock::getDataString(int index) { return getData<const char*>(index); }

#endif // DIFK_BLOCK_H
