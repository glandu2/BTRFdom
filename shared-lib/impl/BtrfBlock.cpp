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
	if(fieldInfo->getHasVariableSize() == false)
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
	checkIndexType(ET_Template, -1);

	reinterpret_cast<std::deque<BtrfBlock*>*>(data)->push_back(static_cast<BtrfBlock*>(block));
	numElement = reinterpret_cast<std::deque<BtrfBlock*>*>(data)->size();
	return numElement - 1;
}

BtrfBlock* BtrfBlock::getBlock(int index) {
	checkIndexType(ET_Template, index);
	return reinterpret_cast<std::deque<BtrfBlock*>*>(data)->at(index);
}

bool BtrfBlock::checkIndexType(int type, int index) {
	if(getType() != type &&
			!(type == ET_Template && getType() == ET_TemplateArray) &&
			!(type == ET_TemplateArray && getType() == ET_Template) &&
			!(type == ET_DWord && getType() == ET_String))
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

void BtrfBlock::dumpToStdout() {
	int i;

	std::cout << std::resetiosflags((std::ios_base::fmtflags)-1);
	//std::cout << getName() << " Block with " << numElement << " elements: ";

	switch(fieldInfo->getType()) {
	case ET_Char:
		std::cout << "Char " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << (int)getDataChar(i) << ", ";
		break;

	case ET_UChar:
		std::cout << "UChar " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << (unsigned int)getDataChar(i) << ", ";
		break;

	case ET_Word:
		std::cout << "Word " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << getDataShort(i) << ", ";
		break;

	case ET_DWord:
		std::cout << "DWord " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << getDataInt(i) << ", ";
		break;

	case ET_Float:
		std::cout << "Float " << getName() << "[" << numElement << "] = " << std::setprecision(5);
		for(i=0; i<numElement; i++)
			std::cout << getDataFloat(i) << ", ";
		break;

	case ET_String:
		std::cout << "String " << getName() << "[" << numElement << "] = \n{\n";
		for(i=0; i<numElement; i++) {
			if(getDataString(i))
				std::cout << getDataString(i) << '\n';
			else std::cout << "<NULL>\n";
		}
		std::cout << "}";
		break;

	case ET_Template: {
		const TemplateGuid* templateGuid = &getTemplateGuid();

		std::cout << "Template GUID = " << std::hex << std::uppercase <<
					 (unsigned int)templateGuid->Data1 << '-' << (unsigned int)templateGuid->Data2 << '-' << (unsigned int)templateGuid->Data3 << '-' <<
					 (unsigned int)templateGuid->Data4[0] << (unsigned int)templateGuid->Data4[1] << '-' << (unsigned int)templateGuid->Data4[2] <<
					 (unsigned int)templateGuid->Data4[3] << (unsigned int)templateGuid->Data4[4] << (unsigned int)templateGuid->Data4[5] <<
					 (unsigned int)templateGuid->Data4[6] << (unsigned int)templateGuid->Data4[7] << std::dec << " " << getName() << ", " << numElement << " subfields\n{\n";
		for(i=0; i<numElement; i++)
			getBlock(i)->dumpToStdout();
		std::cout << "}";
		break;
	}

	case ET_TemplateArray:
		std::cout << "Template array " << getName() << "[" << numElement << "] = \n{\n";
		for(i=0; i<numElement; i++)
			getBlock(i)->dumpToStdout();
		std::cout << "}";
		break;

	case ET_Array:
	case ET_None:
		std::cerr << "Internal error, magic is None or Array\n";
		exit(-1);
	}
	std::cout << std::resetiosflags((std::ios_base::fmtflags)-1);

	std::cout << '\n';
}
