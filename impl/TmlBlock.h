#ifndef TML_BLOCK_H
#define TML_BLOCK_H

#include "ITmlBlock.h"
#include <deque>
#include <string>
#include <stdio.h>

class TmlFile;

class TmlBlock : public ITmlBlock
{
public:
	TmlBlock();
	~TmlBlock();

	COM_BEGIN_DECLARE_IFACE
	COM_DECLARE_IFACE(ITmlBlock)
	COM_END_DECLARE_IFACE

	virtual bool DLLCALLCONV getIsValid() { return isValid; }
	virtual const TemplateGuid& DLLCALLCONV getTemplateGuid() { return guid; }
	virtual const char* DLLCALLCONV getName() { return name.c_str(); }
	virtual TmlBlock* DLLCALLCONV getField(int i) { return subfields.at(i); }
	virtual int DLLCALLCONV getFieldCount() { return numElement; }
	virtual void DLLCALLCONV setFieldCount(int num) { numElement = num; }
	virtual ElementType DLLCALLCONV getType() { return elementType; }
	virtual bool DLLCALLCONV getHasVariableSize() { return hasVariableSize; }

public:
	bool parseFile(FILE *file, TmlFile *tmlFile);

protected:
private:
	bool isValid;
	std::string name;
	TemplateGuid guid;
	std::deque<TmlBlock*> subfields;
	ElementType elementType;
	int numElement;
	bool hasVariableSize;
};

#endif // TML_BLOCK_H
