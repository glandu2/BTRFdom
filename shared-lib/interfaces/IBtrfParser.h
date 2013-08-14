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

#ifndef IBTRFPARSER_H
#define IBTRFPARSER_H

#include "IObject.h"
#include "MemberType.h"

class IBtrfRootBlock;
class ITmlFile;

// {1627C9F4-F71C-4723-9274-011D54DD7D8A}
static const Guid IID_IBtrfParser =
{ 0x1627c9f4, 0xf71c, 0x4723, { 0x92, 0x74, 0x1, 0x1d, 0x54, 0xdd, 0x7d, 0x8a } };


class IBtrfParser : public CImplement<IObject>
{
public:
	virtual IBtrfRootBlock * DLLCALLCONV readFile(const char* filename) = 0;
	virtual void DLLCALLCONV writeFile(const char* filename, IBtrfRootBlock *rootBlock) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF IBtrfParser * DLLCALLCONV createBtrfParser(ITmlFile *tmlFile);

#ifdef __cplusplus
}
#endif


#endif // IBTRFPARSER_H
