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

#ifndef IBTRFROOTBLOCK_H
#define IBTRFROOTBLOCK_H

#include "IObject.h"
#include "MemberType.h"

class IBtrfBlock;
class ITmlFile;

// {C506D3CF-7636-414d-B554-519CD78B2401}
static const Guid IID_IBtrfRootBlock =
{ 0xc506d3cf, 0x7636, 0x414d, { 0xb5, 0x54, 0x51, 0x9c, 0xd7, 0x8b, 0x24, 0x1 } };

class IBtrfRootBlock : public CImplement<IObject>
{
public:
	virtual void DLLCALLCONV addString(const char* str) = 0;
	virtual const char * DLLCALLCONV getString(int index) = 0;

	virtual void DLLCALLCONV addTemplate(const TemplateGuid& guid, int usedField) = 0;
	virtual const TemplateGuid& DLLCALLCONV getTemplateGuid(int index) = 0;
	virtual int DLLCALLCONV getTemplateUsedField(int index) = 0;
	virtual int DLLCALLCONV getStringNum() = 0;
	virtual int DLLCALLCONV getTemplateNum() = 0;

	virtual ITmlFile * DLLCALLCONV getTmlFile() = 0;

	virtual void DLLCALLCONV addBlock(IBtrfBlock *block) = 0;
	virtual IBtrfBlock * DLLCALLCONV getBlock(const TemplateGuid& guid) = 0;
	virtual IBtrfBlock * DLLCALLCONV getBlock(int id) = 0;
	virtual int DLLCALLCONV getBlockNum() = 0;

	virtual void DLLCALLCONV dumpToStdout() = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF IBtrfRootBlock * DLLCALLCONV createBtrfRootBlock(ITmlFile *tmlFile);

#ifdef __cplusplus
}
#endif


#endif // IBTRFROOTBLOCK_H
