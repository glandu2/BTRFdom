#ifndef BTRF_ROOTBLOCK_H
#define BTRF_ROOTBLOCK_H

#include <unordered_map>
#include <deque>
#include <string>

#include "IBtrfRootBlock.h"
#include "TmlFile.h"
#include "BtrfBlock.h"

class BtrfRootBlock : public IBtrfRootBlock
{
public:
	BtrfRootBlock(TmlFile *tmlFile);

	COM_BEGIN_DECLARE_IFACE
	COM_DECLARE_IFACE(IBtrfRootBlock)
	COM_END_DECLARE_IFACE

	virtual void DLLCALLCONV addString(const char* str);
	virtual const char * DLLCALLCONV getString(int index);

	virtual void DLLCALLCONV addTemplate(TemplateGuid guid, int usedField);
	virtual TemplateGuid DLLCALLCONV getTemplateGuid(int index);
	virtual int DLLCALLCONV getTemplateUsedField(int index);
	virtual int DLLCALLCONV getStringNum() { return stringList.size(); }
	virtual int DLLCALLCONV getTemplateNum() { return templateList.size(); }

	virtual TmlFile * DLLCALLCONV getTmlFile() { return tmlFile; }

	virtual void DLLCALLCONV addBlock(IBtrfBlock *block);
	virtual BtrfBlock * DLLCALLCONV getBlock(TemplateGuid guid) { try {return blocks.find(guid)->second; } catch(...) { return 0; } }
	virtual BtrfBlock * DLLCALLCONV getBlock(int id) { return blockList.at(id); }
	virtual int DLLCALLCONV getBlockNum() { return blockList.size(); }

public:

	void dumpToStdout();

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
