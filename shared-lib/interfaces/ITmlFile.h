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

#ifndef ITMLFILE_H
#define ITMLFILE_H

#include "IObject.h"
#include "MemberType.h"

class ITmlBlock;

// {DE4BF2A8-44CF-41ff-A17C-07C95468515A}
static const Guid IID_ITmlFile =
{ 0xde4bf2a8, 0x44cf, 0x41ff, { 0xa1, 0x7c, 0x7, 0xc9, 0x54, 0x68, 0x51, 0x5a } };


class ITmlFile : public IObject
{
public:
	virtual bool DLLCALLCONV parseFile(const char *file) = 0;
	virtual ITmlBlock* DLLCALLCONV getTemplateByGuid(const TemplateGuid& guid) = 0;
	virtual ITmlBlock* DLLCALLCONV getTemplateByName(const char* name) = 0;

	virtual void DLLCALLCONV addTemplate(ITmlBlock *templateStruct) = 0;

	virtual void DLLCALLCONV loadNx3() = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF ITmlFile * DLLCALLCONV createTmlFile();

#ifdef __cplusplus
}
#endif


#endif // ITMLFILE_H
