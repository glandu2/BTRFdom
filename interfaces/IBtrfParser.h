#ifndef IBTRFPARSER_H
#define IBTRFPARSER_H

#include "IObject.h"
#include "MemberType.h"

class IBtrfRootBlock;
class ITmlFile;

// {1627C9F4-F71C-4723-9274-011D54DD7D8A}
static const Guid IID_IBtrfParser =
{{ 0x1627c9f4, 0xf71c, 0x4723, { 0x92, 0x74, 0x1, 0x1d, 0x54, 0xdd, 0x7d, 0x8a } }};


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
