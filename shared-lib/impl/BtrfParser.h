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

#ifndef BTRF_PARSER_H
#define BTRF_PARSER_H

#include "IBtrfParser.h"
#include "File.h"
#include <vector>

#include "BtrfRootBlock.h"

class TmlFile;
class TmlBlock;
class BtrfBlock;

class BtrfParser : public IBtrfParser
{
public:
	BtrfParser(TmlFile *tmlData) : tmlData(tmlData) {}

	COM_BEGIN_DECLARE_IFACE
	COM_DECLARE_IFACE(IBtrfParser)
	COM_END_DECLARE_IFACE

	virtual BtrfRootBlock * DLLCALLCONV readFile(const char* filename);
	virtual void DLLCALLCONV writeFile(const char* filename, IBtrfRootBlock *iRootBlock);

protected:
	BtrfBlock *readBlock(BtrfBlock* block, TmlBlock *tmlField);
	void writeBlock(FILE* file, BtrfBlock *block);

private:
	struct GlobalHeader  {
		char btrf[4];
		int header_size;	//always 4
		short major_version;
		short minor_version;
	};

	const GlobalHeader *header;

	BtrfRootBlock *rootBlock;
	File *file;
	TmlFile *tmlData;

};

#endif // BTRF_PARSER_H
