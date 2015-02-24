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

#include "BtrfBlock.h"
#include <stdlib.h>
#include "TmlBlock.h"
#include "BtrfRootBlock.h"
#include <deque>

template<typename T> struct TypeValue{ enum { MemberType = ET_None }; };
template<> struct TypeValue<char> { enum { MemberType = ET_Char }; };
template<> struct TypeValue<short> { enum { MemberType = ET_Word }; };
template<> struct TypeValue<int> { enum { MemberType = ET_DWord }; };
template<> struct TypeValue<float> { enum { MemberType = ET_Float }; };


BtrfBlock::BtrfBlock(TmlBlock *fieldInfo, BtrfRootBlock *rootBlock)
	: fieldInfo(fieldInfo),
	  rootBlock(rootBlock),
	  data(nullptr),
	  allocatedData(false),
	  templateId(-1)
{
	if(fieldInfo->getHasVariableSize() == false && getType() != ET_Template && getType() != ET_TemplateArray)
		numElement = fieldInfo->getFieldCount();
	else
		numElement = 0;

	initData();
}

BtrfBlock::~BtrfBlock()
{
	freeData();
}

void BtrfBlock::initData() {
//	if(dataType != fieldInfo->getType()) {
//		fprintf(stderr, "FATAL ! Type is not the same as template file: set %d instead of %d\n", dataType, fieldInfo->getType());
//		abort();
//	}

	freeData();

	switch(fieldInfo->getType()) {
	case ET_TemplateArray:
	case ET_Template:
	case ET_Dict:
		this->data = new std::deque<BtrfBlock*>();
		this->allocatedData = true;
		break;

	case ET_Char:
	case ET_UChar:
		if(numElement) {
			this->data = new char[numElement];
			this->allocatedData = true;
		}
		break;

	case ET_Word:
		if(numElement) {
			this->data = new short[numElement];
			this->allocatedData = true;
		}
		break;

	case ET_DWord:
		if(numElement) {
			this->data = new int[numElement];
			this->allocatedData = true;
		}
		break;

	case ET_Float:
		if(numElement) {
			this->data = new float[numElement];
			this->allocatedData = true;
		}
		break;

	case ET_String:
		if(numElement) {
			this->data = new int[numElement];
			this->allocatedData = true;
		}
		break;

	case ET_Array:
	case ET_None:
		this->data = nullptr;
		this->allocatedData = false;
		break;
	}
}

void BtrfBlock::freeData() {
	if(allocatedData) {
		switch(fieldInfo->getType()) {
		case ET_TemplateArray:
		case ET_Template:
		case ET_Dict:
			delete reinterpret_cast<std::deque<BtrfBlock*>*>(data);
			allocatedData = false;
			break;

		case ET_Char:
		case ET_UChar:
			delete[] reinterpret_cast<char*>(data);
			allocatedData = false;
			break;

		case ET_Word:
			delete[] reinterpret_cast<short*>(data);
			allocatedData = false;
			break;

		case ET_DWord:
			delete[] reinterpret_cast<int*>(data);
			allocatedData = false;
			break;

		case ET_Float:
			delete[] reinterpret_cast<float*>(data);
			allocatedData = false;
			break;

		case ET_String:
			delete[] reinterpret_cast<int*>(data);
			allocatedData = false;
			break;

		case ET_Array:
		case ET_None:
			break;
		}
	}

	if(allocatedData == false)
		data = nullptr;
}

void DLLCALLCONV BtrfBlock::setElementNumber(int num) {
	freeData();
	numElement = num;
	initData();
}

template<typename T> void BtrfBlock::setData(int index, T data) {
	checkIndexType(TypeValue<T>::MemberType, index);
	getDataPtr<T>()[index] = data;
}

void DLLCALLCONV BtrfBlock::setDataString(int index, const char* data) {
	for(int i = 0; i < rootBlock->getStringNum(); i++) {
		if(strcmp(rootBlock->getString(i), data) == 0) {
			setDataStringId(index, i);
			return;
		}
	}

	setDataStringId(index, rootBlock->addString(data));
}

template<typename T> T BtrfBlock::getData(int index) {
	checkIndexType(TypeValue<T>::MemberType, index);
	return getDataPtr<T>()[index];
}

template<> const char * BtrfBlock::getData<const char*>(int index) {
	checkIndexType(ET_String, index);

	int id = static_cast<int*>(data)[index];
	if(id != -1)
		return rootBlock->getString(id);
	else
		return nullptr;
}

template<typename T> void BtrfBlock::setDataPtr(T* data) {
	freeData();
	checkIndexType(TypeValue<T>::MemberType, -1);
	this->data = data;
}

template<typename T> T* BtrfBlock::getDataPtr() {
	return static_cast<T*>(data);
}

int BtrfBlock::addBlock(IBtrfBlock *block) {
	if(block->getType() == ET_Template)
		checkIndexType(ET_TemplateArray, -1);
	else if(getType() != ET_Dict)
		checkIndexType(ET_Template, -1);

	if(block->getType() == ET_TemplateArray && block->getFieldInfo()->getHasVariableSize() && block->getTemplateId() == -1) {
		for(int i = 0; i < rootBlock->getTemplateNum(); i++) {
			if(rootBlock->getTemplateGuid(i) == block->getFieldInfo()->getField(0)->getTemplateGuid()) {
				block->setTemplateId(i);
				break;
			}
		}
		int usedField;
		if(block->getElementNumber() > 0) {
			usedField = block->getBlock(0)->getElementNumber();
		} else usedField = block->getFieldInfo()->getField(0)->getFieldCount();

		if(block->getTemplateId() == -1)
			block->setTemplateId(rootBlock->addTemplate(block->getFieldInfo()->getField(0)->getTemplateGuid(), usedField));
	}

	reinterpret_cast<std::deque<BtrfBlock*>*>(data)->push_back(static_cast<BtrfBlock*>(block));
	numElement = (int)reinterpret_cast<std::deque<BtrfBlock*>*>(data)->size();
	return numElement - 1;
}

BtrfBlock* BtrfBlock::getBlock(int index) {
	if(getType() == ET_Template)
		checkIndexType(ET_Template, index);
	else if(getType() == ET_TemplateArray)
		checkIndexType(ET_TemplateArray, index);
	else
		checkIndexType(ET_Dict, index);

	return reinterpret_cast<std::deque<BtrfBlock*>*>(data)->at(index);
}

bool BtrfBlock::checkIndexType(int type, int index) {
	if(getType() != type && !(type == ET_DWord && getType() == ET_String))
	{
		std::cerr << "Warning: attempting to get wrong type to block ! requested type was " << type << " but block is of type " << getType() << '\n';
	}
	if(index >= numElement) {
		std::cerr << "Index too large for member " << getName() << " index " << index + 1 << " / " << numElement << '\n';
		exit(-2);
		return false;
	}
	return true;
}

const TemplateGuid& BtrfBlock::getTemplateGuid() {
	if(templateId != -1)
		return rootBlock->getTemplateGuid(templateId);
	else
		return fieldInfo->getTemplateGuid();
}

void insert_tab(FILE* fout, int indentation) {
	int i;

	for(i = 0; i < indentation; i++)
		fprintf(fout, "\t");
}

void BtrfBlock::dumpToStdout(FILE *fout, int indentation) {
	int i;

	insert_tab(fout, indentation);

	switch(fieldInfo->getType()) {
	case ET_Char:
		fprintf(fout, "Char %s[%d] = ", getName(), numElement);
		for(i=0; i<numElement; i++) {
			fprintf(fout, "\t%d,\n ", (int)getDataShort(i));
			insert_tab(fout, indentation);
		}
		break;

	case ET_UChar:
		fprintf(fout, "UChar %s[%d] = ", getName(), numElement);
		for(i=0; i<numElement; i++) {
			fprintf(fout, "\t%u,\n ", (unsigned int)getDataShort(i));
			insert_tab(fout, indentation);
		}
		break;

	case ET_Word:
		fprintf(fout, "Word %s[%d] = \n", getName(), numElement);
		for(i=0; i<numElement; i++) {
			fprintf(fout, "\t%d,\n ", (int)getDataShort(i));
			insert_tab(fout, indentation);
		}
		break;

	case ET_DWord:
		fprintf(fout, "DWord %s[%d] = ", getName(), numElement);
		for(i=0; i<numElement; i++) {
			fprintf(fout, "\t%d,\n ", (int)getDataShort(i));
			insert_tab(fout, indentation);
		}
		break;

	case ET_Float:
		fprintf(fout, "Float %s[%d] = ", getName(), numElement);
		for(i=0; i<numElement; i++)
			fprintf(fout, "%.5f, ", getDataFloat(i));
		break;

	case ET_String:
		fprintf(fout, "String %s[%d] = \n", getName(), numElement);
		insert_tab(fout, indentation);
		fprintf(fout, "{\n");
		for(i=0; i<numElement; i++) {
			insert_tab(fout, indentation+1);
			if(getDataString(i))
				fprintf(fout, "\"%s\"\n", getDataString(i));
			else fprintf(fout, "<NULL>\n");
		}
		insert_tab(fout, indentation);
		fprintf(fout, "}");
		break;

	case ET_Template: {
		const TemplateGuid* templateGuid = &getTemplateGuid();

		fprintf(fout, "Template GUID = %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X %s, %d subfields\n",
					 (unsigned int)templateGuid->Data1,    (unsigned int)templateGuid->Data2,    (unsigned int)templateGuid->Data3,
					 (unsigned int)templateGuid->Data4[0], (unsigned int)templateGuid->Data4[1], (unsigned int)templateGuid->Data4[2],
					 (unsigned int)templateGuid->Data4[3], (unsigned int)templateGuid->Data4[4], (unsigned int)templateGuid->Data4[5],
					 (unsigned int)templateGuid->Data4[6], (unsigned int)templateGuid->Data4[7], getName(), numElement);

		insert_tab(fout, indentation);
		fprintf(fout, "{\n");

		for(i=0; i<numElement; i++)
			getBlock(i)->dumpToStdout(fout, indentation+1);

		insert_tab(fout, indentation);
		fprintf(fout, "}");
		break;
	}

	case ET_TemplateArray:
		fprintf(fout, "Template array %s[%d] = \n", getName(), numElement);
		insert_tab(fout, indentation);
		fprintf(fout, "{\n");

		for(i=0; i<numElement; i++)
			getBlock(i)->dumpToStdout(fout, indentation+1);

		insert_tab(fout, indentation);
		fprintf(fout, "}");
		break;

	case ET_Dict:
		fprintf(fout, "dict %s = \n", getName());
		insert_tab(fout, indentation);
		fprintf(fout, "{\n");

		for(i=0; i<numElement; i++)
			getBlock(i)->dumpToStdout(fout, indentation+1);

		insert_tab(fout, indentation);
		fprintf(fout, "}");
		break;

	case ET_Array:
	case ET_None:
		fprintf(stderr, "Internal error, magic is None or Array\n");
		exit(-1);
	}

	fprintf(fout, "\n");
}
