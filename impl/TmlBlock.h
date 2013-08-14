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
	virtual TemplateGuid DLLCALLCONV getTemplateGuid() { return guid; }
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
