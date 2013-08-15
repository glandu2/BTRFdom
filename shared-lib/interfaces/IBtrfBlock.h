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

#ifndef IBTRFBLOCK_H
#define IBTRFBLOCK_H

#include "IObject.h"
#include "MemberType.h"

class ITmlBlock;
class IBtrfRootBlock;

// {E109854D-B397-4e02-B76A-E702FBC7B1CF}
static const Guid IID_IBtrfBlock =
{ 0xe109854d, 0xb397, 0x4e02, { 0xb7, 0x6a, 0xe7, 0x2, 0xfb, 0xc7, 0xb1, 0xcf } };


class IBtrfBlock : public IObject
{
public:
	virtual void DLLCALLCONV setElementNumber(int num) = 0;
	virtual void DLLCALLCONV setTemplateId(int id) = 0;

	virtual ITmlBlock * DLLCALLCONV getFieldInfo() = 0;
	virtual ElementType DLLCALLCONV getType() = 0;
	virtual const char* DLLCALLCONV getName() = 0;
	virtual int DLLCALLCONV getElementNumber() = 0;
	virtual int DLLCALLCONV getTemplateId() = 0;
	virtual const TemplateGuid& DLLCALLCONV getTemplateGuid() = 0;

	virtual int DLLCALLCONV addBlock(IBtrfBlock *block) = 0;
	virtual IBtrfBlock* DLLCALLCONV getBlock(int index) = 0;

	virtual void DLLCALLCONV setDataChar(int index, char data) = 0;
	virtual void DLLCALLCONV setDataShort(int index, short data) = 0;
	virtual void DLLCALLCONV setDataInt(int index, int data) = 0;
	virtual void DLLCALLCONV setDataFloat(int index, float data) = 0;
	virtual void DLLCALLCONV setDataStringId(int index, int id) = 0;

	virtual void DLLCALLCONV setDataCharPtr(char *data) = 0;
	virtual void DLLCALLCONV setDataShortPtr(short *data) = 0;
	virtual void DLLCALLCONV setDataIntPtr(int *data) = 0;
	virtual void DLLCALLCONV setDataFloatPtr(float *data) = 0;
	virtual void DLLCALLCONV setDataStringIdPtr(int *id) = 0;

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
};

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF IBtrfBlock * DLLCALLCONV createBtrfBlock(ITmlBlock *fieldInfo, IBtrfRootBlock *rootBlock);

#ifdef __cplusplus
}
#endif

#endif // IBTRFBLOCK_H
