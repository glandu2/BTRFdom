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

#include <stdlib.h>
#include <stdio.h>
#include "IBtrfParser.h"
#include "IBtrfRootBlock.h"
#include "IBtrfBlock.h"
#include "ITmlFile.h"
#include "ITmlBlock.h"
#include <sstream>
#include "gtest/gtest.h"


static const char tml_test[] =
        "template template_dynamic_array\n"
        "{00000000-0000-0000-0000-000000000001}\n"
        "{\n"
        "	dword           c_size;\n"
        "	char            c[c_size];\n"
        "	\n"
        "	dword           w_size;\n"
        "    word            w[w_size];\n"
        "	\n"
        "	dword           d_size;\n"
        "    dword           d[d_size];\n"
        "	\n"
        "	dword           f_size;\n"
        "    float           f[f_size];\n"
        "	\n"
        "	dword           s_size;\n"
        "    string          s[s_size];\n"
        "	\n"
        "	dword           sda_size;\n"
        "    template_dynamic_array sda[sda_size];\n"
        "}\n"
        "\n"
        "template template_fixed_array\n"
        "{00000000-0000-0000-0000-000000000003}\n"
        "{\n"
        "	char c[3];\n"
        "	word w[2];\n"
        "	dword d[3];\n"
        "	float f[2];\n"
        "	string s[3];\n"
        "	template_dynamic_array sda[2];\n"
        "}\n"
        "\n"
        "template header\n"
        "{00000000-0000-0000-0000-000000000002}\n"
        "{\n"
        "	char c;\n"
        "	word w;\n"
        "	dword d;\n"
        "	float f;\n"
        "	string s;\n"
        "//	dict di;\n"
        "	template_dynamic_array sda;\n"
        "	template_fixed_array sfa;\n"
        "}\n";

static const Guid tml_null_guid = { 0x00000000, 0x0000, 0x0000, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static const Guid tml_template_dynamic_array = { 0x00000000, 0x0000, 0x0000, { 0, 0, 0, 0, 0, 0, 0, 1 } };
static const Guid tml_template_fixed_array = { 0x00000000, 0x0000, 0x0000, { 0, 0, 0, 0, 0, 0, 0, 3 } };
static const Guid tml_header = { 0x00000000, 0x0000, 0x0000, { 0, 0, 0, 0, 0, 0, 0, 2 } };

static const TemplateGuid tml_templateguid_null_guid = TemplateGuid(tml_null_guid);
static const TemplateGuid tml_templateguid_template_dynamic_array = TemplateGuid(tml_template_dynamic_array);
static const TemplateGuid tml_templateguid_template_fixed_array = TemplateGuid(tml_template_fixed_array);
static const TemplateGuid tml_templateguid_header = TemplateGuid(tml_header);


const unsigned char btrfBinary[406] =
{
    0x42, 0x54, 0x52, 0x46, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x24, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x06, 0x00, 0x07, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x73, 0x64, 0x63, 0x66,
    0x76, 0x67, 0x68, 0x62, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x70, 0x61, 0x7a, 0x6a, 0x6f,
    0x7a, 0x00, 0x65, 0x6f, 0x72, 0x69, 0x7a, 0x65, 0x66, 0x20, 0x7a, 0x65, 0x66, 0x00, 0x7a, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x2e, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09,
    0x22, 0x01, 0x00, 0x00, 0x02, 0x00, 0x63, 0x1d, 0x16, 0xd7, 0x66, 0x09, 0x00, 0xec, 0x51, 0xa8,
    0x40, 0x01, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x66, 0x67,
    0x68, 0x05, 0x00, 0x00, 0x00, 0x02, 0x39, 0x30, 0x7e, 0xfd, 0x0d, 0x00, 0x00, 0x00, 0x03, 0xa8,
    0x7d, 0xb6, 0x1d, 0xe2, 0xb6, 0xe1, 0xff, 0x66, 0x59, 0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0xa0, 0x3f, 0xe4, 0xd0, 0x8c, 0xd0, 0x0d, 0x00, 0x00, 0x00, 0x07, 0x02, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x02, 0x01, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x07,
    0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00,
    0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x07, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x71, 0x00, 0x00, 0x00, 0x66, 0x67, 0x68, 0x39, 0x30, 0x7e, 0xfd, 0xa8, 0x7d, 0xb6, 0x1d,
    0xe2, 0xb6, 0xe1, 0xff, 0x66, 0x59, 0x01, 0x00, 0x00, 0x00, 0xa0, 0x3f, 0xe4, 0xd0, 0x8c, 0xd0,
    0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x03, 0x01,
    0x00, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x07, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00,
    0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x07, 0x06, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

TEST(tml_full, test_tml_simple_data)
{
	/* Common BTRF / DIFK model:
	 * Element:
	 *  * ElementType
	 *  * Guid
	 *  * Value or Values or child or children
	 */

	std::istringstream tml_test_data(tml_test);

	ITmlFile *tmlFile = createTmlFile();
	tmlFile->parseStream(&tml_test_data);

	ITmlBlock* tmlHeaderBlock = tmlFile->getTemplateByName("header");

	EXPECT_EQ(TemplateGuid(tml_header), tmlHeaderBlock->getTemplateGuid());
	EXPECT_STREQ("header",              tmlHeaderBlock->getName());
	EXPECT_EQ(7,                        tmlHeaderBlock->getFieldCount());
	EXPECT_EQ(ET_Template,              tmlHeaderBlock->getType());
	EXPECT_EQ(false,                    tmlHeaderBlock->getHasVariableSize());

	size_t fieldIndex = 0;
	ITmlBlock* headerCInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerWInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerDInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerFInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSInfo = tmlHeaderBlock->getField(fieldIndex++);
	//ITmlBlock* headerDiInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSdaArrayInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSfaArrayInfo = tmlHeaderBlock->getField(fieldIndex++);

	ITmlBlock* headerSdaInfo = headerSdaArrayInfo->getField(0);
	ITmlBlock* headerSfaInfo = headerSfaArrayInfo->getField(0);

	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerCInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerWInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerDInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerFInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerSInfo->getTemplateGuid());
	//EXPECT_EQ(TemplateGuid(tml_null_guid),       headerDiInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid), headerSdaArrayInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_template_dynamic_array), headerSdaInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_template_fixed_array),   headerSfaInfo->getTemplateGuid());

	EXPECT_STREQ("c", headerCInfo->getName());
	EXPECT_STREQ("w", headerWInfo->getName());
	EXPECT_STREQ("d", headerDInfo->getName());
	EXPECT_STREQ("f", headerFInfo->getName());
	EXPECT_STREQ("s", headerSInfo->getName());
	//EXPECT_STREQ("", headerDiInfo->getName());
	EXPECT_STREQ("sda", headerSdaArrayInfo->getName());
	EXPECT_STREQ("sfa", headerSfaArrayInfo->getName());
	EXPECT_STREQ("template_dynamic_array", headerSdaInfo->getName());
	EXPECT_STREQ("template_fixed_array", headerSfaInfo->getName());

	EXPECT_EQ(1, headerCInfo->getFieldCount());
	EXPECT_EQ(1, headerWInfo->getFieldCount());
	EXPECT_EQ(1, headerDInfo->getFieldCount());
	EXPECT_EQ(1, headerFInfo->getFieldCount());
	EXPECT_EQ(1, headerSInfo->getFieldCount());
	//EXPECT_EQ(0, headerDiInfo->getFieldCount());
	EXPECT_EQ(1, headerSdaArrayInfo->getFieldCount());
	EXPECT_EQ(1, headerSfaArrayInfo->getFieldCount());
	EXPECT_EQ(6, headerSdaInfo->getFieldCount());
	EXPECT_EQ(6, headerSfaInfo->getFieldCount());

	EXPECT_EQ(ET_Char,     headerCInfo->getType());
	EXPECT_EQ(ET_Word,     headerWInfo->getType());
	EXPECT_EQ(ET_DWord,    headerDInfo->getType());
	EXPECT_EQ(ET_Float,    headerFInfo->getType());
	EXPECT_EQ(ET_String,   headerSInfo->getType());
	//EXPECT_EQ(ET_Dict,     headerDiInfo->getType());
	EXPECT_EQ(ET_TemplateArray, headerSdaArrayInfo->getType());
	EXPECT_EQ(ET_TemplateArray, headerSfaArrayInfo->getType());
	EXPECT_EQ(ET_Template, headerSdaInfo->getType());
	EXPECT_EQ(ET_Template, headerSfaInfo->getType());

	EXPECT_EQ(false, headerCInfo->getHasVariableSize());
	EXPECT_EQ(false, headerWInfo->getHasVariableSize());
	EXPECT_EQ(false, headerDInfo->getHasVariableSize());
	EXPECT_EQ(false, headerFInfo->getHasVariableSize());
	EXPECT_EQ(false, headerSInfo->getHasVariableSize());
	//EXPECT_EQ(true,  headerDiInfo->getHasVariableSize());
	EXPECT_EQ(false, headerSdaArrayInfo->getHasVariableSize());
	EXPECT_EQ(false, headerSfaArrayInfo->getHasVariableSize());
	EXPECT_EQ(true,  headerSdaInfo->getHasVariableSize());
	EXPECT_EQ(false, headerSfaInfo->getHasVariableSize());
}


TEST(tml_full, test_tml_dynamic_array)
{
	std::istringstream tml_test_data(tml_test);

	ITmlFile *tmlFile = createTmlFile();
	tmlFile->parseStream(&tml_test_data);

	ITmlBlock* tmlTemplateBlock = tmlFile->getTemplateByGuid(tml_template_dynamic_array);
	ITmlBlock* tmlHeaderBlock = tmlFile->getTemplateByName("header");

	ASSERT_TRUE(tmlTemplateBlock->getTemplateGuid() == TemplateGuid(tml_template_dynamic_array));

	ITmlBlock* headerSdaArrayInfo = tmlHeaderBlock->getField(5);
	ITmlBlock* headerSdaInfo = headerSdaArrayInfo->getField(0);

	int fieldIndex = 0;
	ITmlBlock* templateCInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* templateWInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* templateDInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* templateFInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* templateSInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* templateSdaArrayInfo = headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* templateSdaInfo =     templateSdaArrayInfo->getField(0);


	EXPECT_EQ(TemplateGuid(tml_null_guid), templateCInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid), templateWInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid), templateDInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid), templateFInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid), templateSInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid), templateSdaArrayInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_template_dynamic_array), templateSdaInfo->getTemplateGuid());

	EXPECT_STREQ("c", templateCInfo->getName());
	EXPECT_STREQ("w", templateWInfo->getName());
	EXPECT_STREQ("d", templateDInfo->getName());
	EXPECT_STREQ("f", templateFInfo->getName());
	EXPECT_STREQ("s", templateSInfo->getName());
	EXPECT_STREQ("sda", templateSdaArrayInfo->getName());
	EXPECT_STREQ("template_dynamic_array", templateSdaInfo->getName());

	EXPECT_EQ(0, templateCInfo->getFieldCount());
	EXPECT_EQ(0, templateWInfo->getFieldCount());
	EXPECT_EQ(0, templateDInfo->getFieldCount());
	EXPECT_EQ(0, templateFInfo->getFieldCount());
	EXPECT_EQ(0, templateSInfo->getFieldCount());
	EXPECT_EQ(0, templateSdaArrayInfo->getFieldCount());
	EXPECT_EQ(6, templateSdaInfo->getFieldCount());

	EXPECT_EQ(ET_Char,     templateCInfo->getType());
	EXPECT_EQ(ET_Word,     templateWInfo->getType());
	EXPECT_EQ(ET_DWord,    templateDInfo->getType());
	EXPECT_EQ(ET_Float,    templateFInfo->getType());
	EXPECT_EQ(ET_String,   templateSInfo->getType());
	EXPECT_EQ(ET_TemplateArray, templateSdaArrayInfo->getType());
	EXPECT_EQ(ET_Template, templateSdaInfo->getType());

	EXPECT_EQ(true, templateCInfo->getHasVariableSize());
	EXPECT_EQ(true, templateWInfo->getHasVariableSize());
	EXPECT_EQ(true, templateDInfo->getHasVariableSize());
	EXPECT_EQ(true, templateFInfo->getHasVariableSize());
	EXPECT_EQ(true, templateSInfo->getHasVariableSize());
	EXPECT_EQ(true, templateSdaArrayInfo->getHasVariableSize());
	EXPECT_EQ(true, templateSdaInfo->getHasVariableSize());
}


TEST(tml_full, test_tml_fixed_array)
{
	std::istringstream tml_test_data(tml_test);

	ITmlFile *tmlFile = createTmlFile();
	tmlFile->parseStream(&tml_test_data);

	ITmlBlock* tmlHeaderBlock = tmlFile->getTemplateByName("template_fixed_array");

	ASSERT_TRUE(tmlHeaderBlock->getTemplateGuid() == TemplateGuid(tml_template_fixed_array));

	size_t fieldIndex = 0;
	ITmlBlock* headerCInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerWInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerDInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerFInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSdaArrayInfo = tmlHeaderBlock->getField(fieldIndex++);

	ITmlBlock* headerSdaInfo = headerSdaArrayInfo->getField(0);

	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerCInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerWInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerDInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerFInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid),       headerSInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_null_guid), headerSdaArrayInfo->getTemplateGuid());
	EXPECT_EQ(TemplateGuid(tml_template_dynamic_array), headerSdaInfo->getTemplateGuid());

	EXPECT_STREQ("c", headerCInfo->getName());
	EXPECT_STREQ("w", headerWInfo->getName());
	EXPECT_STREQ("d", headerDInfo->getName());
	EXPECT_STREQ("f", headerFInfo->getName());
	EXPECT_STREQ("s", headerSInfo->getName());
	EXPECT_STREQ("sda", headerSdaArrayInfo->getName());
	EXPECT_STREQ("template_dynamic_array", headerSdaInfo->getName());

	EXPECT_EQ(3, headerCInfo->getFieldCount());
	EXPECT_EQ(2, headerWInfo->getFieldCount());
	EXPECT_EQ(3, headerDInfo->getFieldCount());
	EXPECT_EQ(2, headerFInfo->getFieldCount());
	EXPECT_EQ(3, headerSInfo->getFieldCount());
	EXPECT_EQ(2, headerSdaArrayInfo->getFieldCount());
	EXPECT_EQ(6, headerSdaInfo->getFieldCount());

	EXPECT_EQ(ET_Char,     headerCInfo->getType());
	EXPECT_EQ(ET_Word,     headerWInfo->getType());
	EXPECT_EQ(ET_DWord,    headerDInfo->getType());
	EXPECT_EQ(ET_Float,    headerFInfo->getType());
	EXPECT_EQ(ET_String,   headerSInfo->getType());
	EXPECT_EQ(ET_TemplateArray, headerSdaArrayInfo->getType());
	EXPECT_EQ(ET_Template, headerSdaInfo->getType());

	EXPECT_EQ(false, headerCInfo->getHasVariableSize());
	EXPECT_EQ(false, headerWInfo->getHasVariableSize());
	EXPECT_EQ(false, headerDInfo->getHasVariableSize());
	EXPECT_EQ(false, headerFInfo->getHasVariableSize());
	EXPECT_EQ(false, headerSInfo->getHasVariableSize());
	EXPECT_EQ(false, headerSdaArrayInfo->getHasVariableSize());
	EXPECT_EQ(true,  headerSdaInfo->getHasVariableSize());
}

template<typename T> void setBlockValue(IBtrfBlock* block, int index, T value);
template<> void setBlockValue<char>(IBtrfBlock* block, int index, char value) { block->setDataChar(index, value); }
template<> void setBlockValue<short>(IBtrfBlock* block, int index, short value) { block->setDataShort(index, value); }
template<> void setBlockValue<int>(IBtrfBlock* block, int index, int value) { block->setDataInt(index, value); }
template<> void setBlockValue<float>(IBtrfBlock* block, int index, float value) { block->setDataFloat(index, value); }
template<> void setBlockValue<const char*>(IBtrfBlock* block, int index, const char* value) { block->setDataString(index, value); }

template<typename T> T getBlockValue(IBtrfBlock* block, int index);
template<> char getBlockValue<char>(IBtrfBlock* block, int index) { return block->getDataChar(index); }
template<> short getBlockValue<short>(IBtrfBlock* block, int index) { return block->getDataShort(index); }
template<> int getBlockValue<int>(IBtrfBlock* block, int index) { return block->getDataInt(index); }
template<> float getBlockValue<float>(IBtrfBlock* block, int index) { return block->getDataFloat(index); }
template<> const char* getBlockValue<const char*>(IBtrfBlock* block, int index) { return block->getDataString(index); }


template<typename T> struct ElementTypeTrait {};

template<> struct ElementTypeTrait<char> { enum { type = ET_Char }; };
template<> struct ElementTypeTrait<short> { enum { type = ET_Word }; };
template<> struct ElementTypeTrait<int> { enum { type = ET_DWord }; };
template<> struct ElementTypeTrait<float> { enum { type = ET_Float }; };
template<> struct ElementTypeTrait<const char*> { enum { type = ET_String }; };

template<typename T>
IBtrfBlock* createValueBlock(ITmlBlock* tmlBlock, IBtrfRootBlock* rootBlock, IBtrfBlock* parentBlock, std::initializer_list<T> value) {
	IBtrfBlock* block = createBtrfBlock(tmlBlock, rootBlock);
	if(tmlBlock->getHasVariableSize()) {
		block->setElementNumber(value.size());
	} else {
		EXPECT_EQ(value.size(), block->getElementNumber());
	}

	for(size_t i = 0; i < value.size(); i++)
		setBlockValue<T>(block, i, value.begin()[i]);
	parentBlock->addBlock(block);
	return block;
}

IBtrfBlock* addDynamicArrayBlock(IBtrfRootBlock* rootBlock, ITmlBlock* headerSdaArrayInfo, size_t size, bool withSubTemplate = true) {
	ITmlBlock* headerSdaInfo = headerSdaArrayInfo->getField(0);

	int fieldIndex = 0;
	ITmlBlock* dynArrayCInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* dynArrayWInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* dynArrayDInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* dynArrayFInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* dynArraySInfo =      headerSdaInfo->getField(fieldIndex++);
	ITmlBlock* dynArraySdaArrayInfo =     headerSdaInfo->getField(fieldIndex++);

	IBtrfBlock* headerSdaArrayBlock = createBtrfBlock(headerSdaArrayInfo, rootBlock);

	for(size_t i = 0; i < size; i++) {
		IBtrfBlock* headerSdaBlock = createBtrfBlock(headerSdaInfo, rootBlock);

		if(withSubTemplate) {
			createValueBlock<char>(dynArrayCInfo, rootBlock, headerSdaBlock, {'f', 'g', 'h'});
			createValueBlock<short>(dynArrayWInfo, rootBlock, headerSdaBlock, {12345, -642});
			createValueBlock<int>(dynArrayDInfo, rootBlock, headerSdaBlock, {498498984, -1984798, 88422});
			createValueBlock<float>(dynArrayFInfo, rootBlock, headerSdaBlock, {1.25f, -1.89e10f});
			createValueBlock<const char*>(dynArraySInfo, rootBlock, headerSdaBlock, {"abcdefpazjoz", "eorizef zef", "z"});
		} else {
			createValueBlock<char>(dynArrayCInfo, rootBlock, headerSdaBlock, {});
			createValueBlock<short>(dynArrayWInfo, rootBlock, headerSdaBlock, {});
			createValueBlock<int>(dynArrayDInfo, rootBlock, headerSdaBlock, {});
			createValueBlock<float>(dynArrayFInfo, rootBlock, headerSdaBlock, {});
			createValueBlock<const char*>(dynArraySInfo, rootBlock, headerSdaBlock, {});
		}

		IBtrfBlock* headerSdaArraySubBlock = addDynamicArrayBlock(rootBlock, dynArraySdaArrayInfo, withSubTemplate ? 2 : 0, false);
		headerSdaBlock->addBlock(headerSdaArraySubBlock);

		headerSdaArrayBlock->addBlock(headerSdaBlock);
	}

	return headerSdaArrayBlock;
}

TEST(tml_full, test_write_btrf)
{
	std::istringstream tml_test_data(tml_test);

	ITmlFile *tmlFile = createTmlFile();
	tmlFile->parseStream(&tml_test_data);

	ITmlBlock* tmlHeaderBlock = tmlFile->getTemplateByName("header");

	size_t fieldIndex = 0;
	ITmlBlock* headerCInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerWInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerDInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerFInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSdaArrayInfo = tmlHeaderBlock->getField(fieldIndex++);
	ITmlBlock* headerSfaArrayInfo = tmlHeaderBlock->getField(fieldIndex++);

	// BTRF construction

	IBtrfRootBlock* rootBlock = createBtrfRootBlock(tmlFile);

	// Header
	IBtrfBlock* headerBlock = createBtrfBlock(tmlHeaderBlock, rootBlock);
	createValueBlock<char>(headerCInfo, rootBlock, headerBlock, {'c'});
	createValueBlock<short>(headerWInfo, rootBlock, headerBlock, {5661});
	createValueBlock<int>(headerDInfo, rootBlock, headerBlock, {616151});
	createValueBlock<float>(headerFInfo, rootBlock, headerBlock, {5.26f});
	createValueBlock<const char*>(headerSInfo, rootBlock, headerBlock, {"sdcfvghb"});

	// Template with dynamic arrays
	IBtrfBlock* headerSdaArrayBlock = addDynamicArrayBlock(rootBlock, headerSdaArrayInfo, 1);
	headerBlock->addBlock(headerSdaArrayBlock);

	// Template with fixed arrays
	IBtrfBlock* headerSfaArrayBlock = addDynamicArrayBlock(rootBlock, headerSfaArrayInfo, 1);
	headerBlock->addBlock(headerSfaArrayBlock);

	rootBlock->addBlock(headerBlock);

	IBtrfParser* parser = createBtrfParser(tmlFile);
	parser->writeFile("test_write.btrf", rootBlock);


	FILE* file = fopen("test_write.btrf", "rb");
	ASSERT_NE(nullptr, file);

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	std::vector<unsigned char> data;
	data.resize(fileSize);
	int nbRead = fread((char*)&data[0], 1, fileSize, file);
	fclose(file);

	ASSERT_EQ(data.size(), nbRead);
	ASSERT_EQ(sizeof(btrfBinary), data.size());

	EXPECT_TRUE(0 == memcmp(data.data(), btrfBinary, sizeof(btrfBinary)));
}


#define SCOPED_CALL(func_call) { SCOPED_TRACE(#func_call); func_call; }

template<typename T>
static void expectValueBlock(IBtrfBlock* parentBlock, int index, const char* name, std::initializer_list<T> value) {
	IBtrfBlock* block;

	if(index >= 0)
		block = parentBlock->getBlock(index);
	else
		block = parentBlock;

	ASSERT_NE(nullptr, block);
	EXPECT_EQ(ElementTypeTrait<T>::type, block->getType());
	EXPECT_STREQ(name, block->getName());
	EXPECT_EQ(value.size(), block->getElementNumber());

	size_t maxSize = value.size() > block->getElementNumber() ? block->getElementNumber() : value.size();
	for(size_t i = 0; i < maxSize; i++) {
		EXPECT_EQ(value.begin()[i], getBlockValue<T>(block, i));
	}

	EXPECT_EQ(-1, block->getTemplateId());
	EXPECT_EQ(tml_templateguid_null_guid, block->getTemplateGuid());
}

static void expectStringValueBlock(IBtrfBlock* parentBlock, int index, const char* name, std::initializer_list<const char*> value, std::initializer_list<int> stringIndex) {
	IBtrfBlock* block;

	if(index >= 0)
		block = parentBlock->getBlock(index);
	else
		block = parentBlock;

	ASSERT_NE(nullptr, block);
	EXPECT_EQ(ET_String, block->getType());
	EXPECT_STREQ(name, block->getName());
	EXPECT_EQ(value.size(), block->getElementNumber());
	ASSERT_EQ(value.size(), stringIndex.size());

	size_t maxSize = value.size() > block->getElementNumber() ? block->getElementNumber() : value.size();
	for(size_t i = 0; i < maxSize; i++) {
		EXPECT_STREQ(value.begin()[i], getBlockValue<const char*>(block, i));
		EXPECT_EQ(stringIndex.begin()[i], block->getDataStringId(i));
	}

	EXPECT_EQ(-1, block->getTemplateId());
	EXPECT_EQ(tml_templateguid_null_guid, block->getTemplateGuid());
}

static void expectTemplateArrayBlock(IBtrfBlock* parentBlock, int index, const char* name, int size, TemplateGuid guid = tml_templateguid_null_guid, int templateId = -1) {
	IBtrfBlock* block;

	if(index >= 0)
		block = parentBlock->getBlock(index);
	else
		block = parentBlock;

	ASSERT_NE(nullptr, block);
	EXPECT_EQ(ET_TemplateArray, block->getType());
	EXPECT_STREQ(name, block->getName());
	EXPECT_EQ(size, block->getElementNumber());
	EXPECT_EQ(templateId, block->getTemplateId());
	EXPECT_EQ(guid, block->getTemplateGuid());
}

static void expectTemplateBlock(IBtrfBlock* parentBlock, int index, const char* name, int size, TemplateGuid guid, int templateId = -1) {
	IBtrfBlock* block;

	if(index >= 0)
		block = parentBlock->getBlock(index);
	else
		block = parentBlock;

	ASSERT_NE(nullptr, block);
	EXPECT_EQ(ET_Template, block->getType());
	EXPECT_STREQ(name, block->getName());
	EXPECT_EQ(size, block->getElementNumber());
	EXPECT_EQ(templateId, block->getTemplateId());
	EXPECT_EQ(guid, block->getTemplateGuid());
}

TEST(tml_full, test_read_btrf)
{
	std::istringstream tml_test_data(tml_test);

	FILE* file = fopen("test_read.btrf", "wb");
	ASSERT_NE(nullptr, file);
	int nbRead = fwrite(btrfBinary, 1, sizeof(btrfBinary), file);
	fclose(file);
	ASSERT_EQ(sizeof(btrfBinary), nbRead);

	ITmlFile *tmlFile = createTmlFile();
	tmlFile->parseStream(&tml_test_data);

	// BTRF construction

	IBtrfParser* parser = createBtrfParser(tmlFile);
	IBtrfRootBlock* rootBlock = parser->readFile("test_read.btrf");


	ASSERT_EQ(4, rootBlock->getStringNum());
	EXPECT_STREQ("sdcfvghb", rootBlock->getString(0));
	EXPECT_STREQ("abcdefpazjoz", rootBlock->getString(1));
	EXPECT_STREQ("eorizef zef", rootBlock->getString(2));
	EXPECT_STREQ("z", rootBlock->getString(3));

	ASSERT_EQ(2, rootBlock->getTemplateNum());
	EXPECT_EQ(tml_templateguid_template_dynamic_array, rootBlock->getTemplateGuid(0));
	EXPECT_EQ(tml_templateguid_header, rootBlock->getTemplateGuid(1));

	ASSERT_EQ(1, rootBlock->getBlockNum());

	// Header
	IBtrfBlock* headerBlock = rootBlock->getBlockById(0);
	EXPECT_EQ(headerBlock, rootBlock->getBlockByGuid(tml_header));

	SCOPED_CALL(expectTemplateBlock(headerBlock, -1, "header", 7, tml_templateguid_header, 1));

	size_t fieldIndex = 0;

	// Value blocks
	SCOPED_CALL(expectValueBlock<char>(headerBlock, fieldIndex++, "c", {'c'}));
	SCOPED_CALL(expectValueBlock<short>(headerBlock, fieldIndex++, "w", {5661}));
	SCOPED_CALL(expectValueBlock<int>(headerBlock, fieldIndex++, "d", {616151}));
	SCOPED_CALL(expectValueBlock<float>(headerBlock, fieldIndex++, "f", {5.26f}));
	SCOPED_CALL(expectStringValueBlock(headerBlock, fieldIndex++, "s", {"sdcfvghb"}, {0}));


	// sda array block
	IBtrfBlock* headerSdaArrayBlock = headerBlock->getBlock(fieldIndex++);
	SCOPED_CALL(expectTemplateArrayBlock(headerSdaArrayBlock, -1, "sda", 1));

	for(int i = 0; i < headerSdaArrayBlock->getElementNumber(); i++) {
		IBtrfBlock* headerSdaBlock = headerSdaArrayBlock->getBlock(i);

		int subFieldIndex = 0;
		SCOPED_CALL(expectValueBlock<char>(headerSdaBlock, subFieldIndex++, "c", {'f', 'g', 'h'}));
		SCOPED_CALL(expectValueBlock<short>(headerSdaBlock, subFieldIndex++, "w", {12345, -642}));
		SCOPED_CALL(expectValueBlock<int>(headerSdaBlock, subFieldIndex++, "d", {498498984, -1984798, 88422}));
		SCOPED_CALL(expectValueBlock<float>(headerSdaBlock, subFieldIndex++, "f", {1.25f, -1.89e10f}));
		SCOPED_CALL(expectStringValueBlock(headerSdaBlock, subFieldIndex++, "s", {"abcdefpazjoz", "eorizef zef", "z"}, {1, 2, 3}));

		IBtrfBlock* subTemplateSdaArrayBlock = headerSdaBlock->getBlock(subFieldIndex++);
		SCOPED_CALL(expectTemplateArrayBlock(subTemplateSdaArrayBlock, -1, "sda", 2, tml_templateguid_template_dynamic_array, 0));

		for(int i = 0; i < subTemplateSdaArrayBlock->getElementNumber(); i++) {
			IBtrfBlock* subTemplateSdaBlock = subTemplateSdaArrayBlock->getBlock(i);

			int subSubFieldIndex = 0;
			SCOPED_CALL(expectValueBlock<char>(subTemplateSdaBlock, subSubFieldIndex++, "c", {}));
			SCOPED_CALL(expectValueBlock<short>(subTemplateSdaBlock, subSubFieldIndex++, "w", {}));
			SCOPED_CALL(expectValueBlock<int>(subTemplateSdaBlock, subSubFieldIndex++, "d", {}));
			SCOPED_CALL(expectValueBlock<float>(subTemplateSdaBlock, subSubFieldIndex++, "f", {}));
			SCOPED_CALL(expectStringValueBlock(subTemplateSdaBlock, subSubFieldIndex++, "s", {}, {}));
			SCOPED_CALL(expectTemplateArrayBlock(subTemplateSdaBlock, subSubFieldIndex++, "sda", 0, tml_templateguid_template_dynamic_array, 0));
		}
	}


	// sfa array block
	IBtrfBlock* headerSfaArrayBlock = headerBlock->getBlock(fieldIndex++);
	SCOPED_CALL(expectTemplateArrayBlock(headerSfaArrayBlock, -1, "sfa", 1));

	for(int i = 0; i < headerSfaArrayBlock->getElementNumber(); i++) {
		IBtrfBlock* headerSfaBlock = headerSfaArrayBlock->getBlock(i);

		int subFieldIndex = 0;
		SCOPED_CALL(expectValueBlock<char>(headerSfaBlock, subFieldIndex++, "c", {'f', 'g', 'h'}));
		SCOPED_CALL(expectValueBlock<short>(headerSfaBlock, subFieldIndex++, "w", {12345, -642}));
		SCOPED_CALL(expectValueBlock<int>(headerSfaBlock, subFieldIndex++, "d", {498498984, -1984798, 88422}));
		SCOPED_CALL(expectValueBlock<float>(headerSfaBlock, subFieldIndex++, "f", {1.25f, -1.89e10f}));
		SCOPED_CALL(expectStringValueBlock(headerSfaBlock, subFieldIndex++, "s", {"abcdefpazjoz", "eorizef zef", "z"}, {1, 2, 3}));

		IBtrfBlock* subTemplateSdaArrayBlock = headerSfaBlock->getBlock(subFieldIndex++);
		SCOPED_CALL(expectTemplateArrayBlock(subTemplateSdaArrayBlock, -1, "sda", 2));

		for(int i = 0; i < subTemplateSdaArrayBlock->getElementNumber(); i++) {
			IBtrfBlock* subTemplateSdaBlock = subTemplateSdaArrayBlock->getBlock(i);

			int subSubFieldIndex = 0;
			SCOPED_CALL(expectValueBlock<char>(subTemplateSdaBlock, subSubFieldIndex++, "c", {}));
			SCOPED_CALL(expectValueBlock<short>(subTemplateSdaBlock, subSubFieldIndex++, "w", {}));
			SCOPED_CALL(expectValueBlock<int>(subTemplateSdaBlock, subSubFieldIndex++, "d", {}));
			SCOPED_CALL(expectValueBlock<float>(subTemplateSdaBlock, subSubFieldIndex++, "f", {}));
			SCOPED_CALL(expectStringValueBlock(subTemplateSdaBlock, subSubFieldIndex++, "s", {}, {}));
			SCOPED_CALL(expectTemplateArrayBlock(subTemplateSdaBlock, subSubFieldIndex++, "sda", 0, tml_templateguid_template_dynamic_array, 0));
		}
	}
}
