#include "BtrfBlock.h"
#include <stdlib.h>
#include "TmlBlock.h"
#include "BtrfRootBlock.h"

char *strdup (const char *s) {
	char *d = (char*)malloc (strlen (s) + 1);   // Space for length plus nul
	if (d == NULL)
		return NULL;                     // No memory
	strcpy (d,s);                        // Copy the characters
	return d;                            // Return the new string
}

void BtrfBlock::setData(ElementType dataType, void *data, int num) {
	if(num)
		numElement = num;

	if(dataType != fieldInfo->getType()) {
		fprintf(stderr, "FATAL ! Type is not the same as template file: set %d instead of %d\n", dataType, fieldInfo->getType());
		abort();
	}

	freeData();

	switch(dataType) {
	case ET_TemplateArray:
	case ET_Template:
		this->data = new BtrfBlock[numElement];
		this->allocatedData = true;
		break;

	case ET_Char:
	case ET_UChar:
		this->data = new char[numElement];
		this->allocatedData = true;
		memcpy(this->data, data, sizeof(char) * numElement);
		break;

	case ET_Word:
		this->data = new short[numElement];
		this->allocatedData = true;
		memcpy(this->data, data, sizeof(short) * numElement);
		break;

	case ET_DWord:
		this->data = new int[numElement];
		this->allocatedData = true;
		memcpy(this->data, data, sizeof(int) * numElement);
		break;

	case ET_Float:
		this->data = new float[numElement];
		this->allocatedData = true;
		memcpy(this->data, data, sizeof(float) * numElement);
		break;

	case ET_String:
		this->data = new int[numElement];
		this->allocatedData = true;
		if(data)
			memcpy(this->data, data, sizeof(int) * numElement);
		break;

	case ET_Array:
	case ET_None:
		this->data = nullptr;
		this->allocatedData = false;
		break;
	}
}

void BtrfBlock::setDataPtr(ElementType dataType, void *data, int num) {
	if(num)
		numElement = num;

	assert(dataType == fieldInfo->getType());

	freeData();

	switch(dataType) {
	case ET_TemplateArray:
	case ET_Template:
		this->data = new BtrfBlock[numElement];
		this->allocatedData = true;
		break;

	case ET_Char:
	case ET_UChar:
	case ET_Word:
	case ET_DWord:
	case ET_Float:
	case ET_String:
		this->data = data;
		this->allocatedData = false;
		break;

	case ET_Array:
	case ET_None:
		this->data = nullptr;
		this->allocatedData = false;
		break;
	}
}

BtrfBlock* BtrfBlock::getBlock(int index) {
	if(index >= numElement) {
		std::cerr << "Index too large for member " << getName() << " index " << index << " / " << numElement << '\n';
		exit(-2);
	}
	return static_cast<BtrfBlock*>(data) + index;
}

void BtrfBlock::freeData() {
	if(allocatedData) {
		switch(fieldInfo->getType()) {
		case ET_TemplateArray:
		case ET_Template:
			delete[] reinterpret_cast<BtrfBlock*>(data);
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

		if(allocatedData == false)
			data = nullptr;
	}
}

TemplateGuid BtrfBlock::getTemplateGuid() {
	if(templateId != -1)
		return rootBlock->getTemplateGuid(templateId);
	else
		return fieldInfo->getTemplateGuid();
}

ElementType BtrfBlock::getType() {
	return fieldInfo->getType();
}
const char* BtrfBlock::getName() {
	return fieldInfo->getName();
}

const void * BtrfBlock::getData(int index) {
	if(allocatedData) {
		switch(fieldInfo->getType()) {
		case ET_TemplateArray:
		case ET_Template:
			return reinterpret_cast<BtrfBlock*>(data) + index;
			break;

		case ET_Char:
		case ET_UChar:
			return reinterpret_cast<char*>(data) + index;
			break;

		case ET_Word:
			return reinterpret_cast<short*>(data) + index;
			break;

		case ET_DWord:
			return reinterpret_cast<int*>(data) + index;
			break;

		case ET_Float:
			return reinterpret_cast<float*>(data) + index;
			break;

		case ET_String:
			if(reinterpret_cast<int*>(data)[index] != -1)
				return rootBlock->getString(reinterpret_cast<int*>(data)[index]);
			else return nullptr;
			break;

		case ET_Array:
		case ET_None:
			break;
		}
	}

	return nullptr;
}

template<>
const char * BtrfBlock::getData<const char*>(int index) {
   if(index >= numElement) {
	   std::cerr << "Index too large for data " << getName() << " index " << index << " / " << numElement << '\n';
	   ::exit(-2);
   }
   int id = static_cast<int*>(data)[index];
   if(id != -1)
	   return rootBlock->getString(id);
   else
	   return nullptr;
}

void BtrfBlock::dumpToStdout() {
	int i;

	std::cout << std::resetiosflags((std::ios_base::fmtflags)-1);
	//std::cout << getName() << " Block with " << numElement << " elements: ";

	switch(fieldInfo->getType()) {
	case ET_Char:
		std::cout << "Char " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << (int)static_cast<char*>(data)[i] << ", ";
		break;

	case ET_UChar:
		std::cout << "UChar " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << (unsigned int)static_cast<unsigned char*>(data)[i] << ", ";
		break;

	case ET_Word:
		std::cout << "Word " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << static_cast<short*>(data)[i] << ", ";
		break;

	case ET_DWord:
		std::cout << "DWord " << getName() << "[" << numElement << "] = ";
		for(i=0; i<numElement; i++)
			std::cout << static_cast<int*>(data)[i] << ", ";
		break;

	case ET_Float:
		std::cout << "Float " << getName() << "[" << numElement << "] = " << std::setprecision(5);
		for(i=0; i<numElement; i++)
			std::cout << static_cast<float*>(data)[i] << ", ";
		break;

	case ET_String:
		std::cout << "String " << getName() << "[" << numElement << "] = \n{\n";
		for(i=0; i<numElement; i++) {
			if(getData<const char*>(i))
				std::cout << getData<const char*>(i) << '\n';
			else std::cout << "<NULL>\n";
		}
		std::cout << "}";
		break;

	case ET_Template: {
		TemplateGuid templateGuid = getTemplateGuid();

		std::cout << "Template GUID = " << std::hex << std::uppercase <<
					 (unsigned int)templateGuid.Data1 << '-' << (unsigned int)templateGuid.Data2 << '-' << (unsigned int)templateGuid.Data3 << '-' <<
					 (unsigned int)templateGuid.Data4[0] << (unsigned int)templateGuid.Data4[1] << '-' << (unsigned int)templateGuid.Data4[2] <<
					 (unsigned int)templateGuid.Data4[3] << (unsigned int)templateGuid.Data4[4] << (unsigned int)templateGuid.Data4[5] <<
					 (unsigned int)templateGuid.Data4[6] << (unsigned int)templateGuid.Data4[7] << std::dec << " " << getName() << ", " << numElement << " subfields\n{\n";
		for(i=0; i<numElement; i++)
			static_cast<BtrfBlock*>(data)[i].dumpToStdout();
		std::cout << "}";
		break;
	}

	case ET_TemplateArray:
		std::cout << "Template array " << getName() << "[" << numElement << "] = \n{\n";
		for(i=0; i<numElement; i++)
			static_cast<BtrfBlock*>(data)[i].dumpToStdout();
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
