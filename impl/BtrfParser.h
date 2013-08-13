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
	struct __attribute__((packed)) GlobalHeader  {
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
