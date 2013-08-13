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

#ifndef ITMLBLOCK_H
#define ITMLBLOCK_H

#include "IObject.h"
#include "MemberType.h"

// {9117640A-7A3E-414e-A6FC-919DA4BE406B}
static const Guid IID_ITmlBlock =
{{ 0x9117640a, 0x7a3e, 0x414e, { 0xa6, 0xfc, 0x91, 0x9d, 0xa4, 0xbe, 0x40, 0x6b } }};


class ITmlBlock : public CImplement<IObject>
{
public:
	virtual bool DLLCALLCONV getIsValid() = 0;
	virtual const TemplateGuid& DLLCALLCONV getTemplateGuid() = 0;
	virtual const char* DLLCALLCONV getName() = 0;
	virtual ITmlBlock* DLLCALLCONV getField(int i) = 0;
	virtual int DLLCALLCONV getFieldCount() = 0;
	virtual void DLLCALLCONV setFieldCount(int num) = 0;
	virtual ElementType DLLCALLCONV getType() = 0;
	virtual bool DLLCALLCONV getHasVariableSize() = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF ITmlBlock * DLLCALLCONV createTmlBlock();

#ifdef __cplusplus
}
#endif


#endif // ITMLBLOCK_H
