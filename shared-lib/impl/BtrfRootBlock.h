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

#ifndef BTRF_ROOTBLOCK_H
#define BTRF_ROOTBLOCK_H

#include <unordered_map>
#include <deque>
#include <string>

#include "IBtrfRootBlock.h"
#include "TmlFile.h"
#include "BtrfBlock.h"

struct TemplateInfo {
	TemplateGuid guid;
	int usedFieldsNum;
};

class BtrfRootBlock : public CImplement<IBtrfRootBlock>
{
public:
	BtrfRootBlock(TmlFile *tmlFile);

	COM_BEGIN_DECLARE_IFACE
	COM_DECLARE_IFACE(IBtrfRootBlock)
	COM_END_DECLARE_IFACE

	virtual int DLLCALLCONV addString(const char* str);
	virtual const char * DLLCALLCONV getString(int index);

	virtual int DLLCALLCONV addTemplate(const TemplateGuid& guid, int usedField);
	virtual const TemplateGuid& DLLCALLCONV getTemplateGuid(int index);
	virtual int DLLCALLCONV getTemplateUsedField(int index);
	virtual int DLLCALLCONV getStringNum() { return stringList.size(); }
	virtual int DLLCALLCONV getTemplateNum() { return templateList.size(); }

	virtual TmlFile * DLLCALLCONV getTmlFile() { return tmlFile; }

	virtual int DLLCALLCONV addBlock(IBtrfBlock *block);
	virtual BtrfBlock * DLLCALLCONV getBlockByGuid(const TemplateGuid& guid) { try {return blocks.find(guid)->second; } catch(...) { return 0; } }
	virtual BtrfBlock * DLLCALLCONV getBlockById(int id) { return blockList.at(id); }
	virtual int DLLCALLCONV getBlockNum() { return blockList.size(); }

	virtual void DLLCALLCONV dumpToStdout();

protected:
private:
	void *fileMemory;
	TmlFile *tmlFile;
	std::deque<BtrfBlock*> blockList;
	std::unordered_multimap<TemplateGuid, BtrfBlock*> blocks;
	std::deque<std::string> stringList;
	std::deque<TemplateInfo> templateList;
};

#endif // BTRF_ROOTBLOCK_H
