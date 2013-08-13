#ifndef ITMLFILE_H
#define ITMLFILE_H

#include "IObject.h"
#include "MemberType.h"

class ITmlBlock;

// {DE4BF2A8-44CF-41ff-A17C-07C95468515A}
static const Guid IID_ITmlFile =
{{ 0xde4bf2a8, 0x44cf, 0x41ff, { 0xa1, 0x7c, 0x7, 0xc9, 0x54, 0x68, 0x51, 0x5a } }};


class ITmlFile : public CImplement<IObject>
{
public:
	virtual bool DLLCALLCONV parseFile(const char *file) = 0;
	virtual ITmlBlock* DLLCALLCONV getTemplate(const TemplateGuid& guid) = 0;
	virtual ITmlBlock* DLLCALLCONV getTemplate(const char* name) = 0;

	virtual void DLLCALLCONV addTemplate(ITmlBlock *templateStruct) = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT_BTRF ITmlFile * DLLCALLCONV createTmlFile();

#ifdef __cplusplus
}
#endif


#endif // ITMLFILE_H
