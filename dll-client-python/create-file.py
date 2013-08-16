#
# BTRFdom - Rappelz BTRF Document Object Model
# By Glandu2
# Copyright 2013 Glandu2
#
# This file is part of BTRFdom.
# BTRFdom is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# BTRFdom is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with BTRFdom.  If not, see <http://www.gnu.org/licenses/>.
#

# This python script create a basic BTRF file

import sys

if sys.platform == 'win32':
	from ctypes import windll
	btrfdll = windll.LoadLibrary("./BTRFdom")
else:
	from ctypes import cdll
	btrfdll = cdll.LoadLibrary("./BTRFdom.so")

from ctypes import c_float
import uuid

#Load the BTRFdom dll (or so on linux)

#Create a TmlFile object that will contain all known templates and their fields
tmlFile = btrfdll.createTmlFile()

#Read some template files
btrfdll.parseFileTmlFile(tmlFile, b"nx3.tml")
btrfdll.parseFileTmlFile(tmlFile, b"nobj.tml")

#Create a root block, it will contain all block in the btrf file (it does not exist in the file, it's here only to represent the file with all its blocks)
rootBlock = btrfdll.createBtrfRootBlock(tmlFile)

#get the GUID of the template we want (here it's a "nx3_mesh_tm", defined in nx3.tml)
blockGuid = uuid.UUID('{F09C560E-7328-411e-87A3-EEB165D5F929}')
#get the template info associated with the given GUID
fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, blockGuid.bytes_le)

#Add the template GUID and the number of used fields to the rootblocks
blockGuidId = btrfdll.addTemplateBtrfRootBlock(rootBlock, blockGuid.bytes_le, 2)

#Create a block that will contain the data of the template
block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)

#Set it's template, the template must be referenced with it's ID
btrfdll.setTemplateIdBtrfBlock(block, blockGuidId)


#Get the first field of the template and create a block with that field (it's the "name" field)
subBlockInfo = btrfdll.getFieldTmlBlock(fieldInfo, 0)
subBlock = btrfdll.createBtrfBlock(subBlockInfo, rootBlock)

#Add a string to the file to reference it after in the new block
stringId = btrfdll.addStringBtrfRootBlock(rootBlock, b"test string")
btrfdll.setDataStringIdBtrfBlock(subBlock, 0, stringId)

#add the string subblock to the template block
btrfdll.addBlockBtrfBlock(block, subBlock)

#Get the second field of the template and create a block with that field (it's the "tm" field)
subBlockInfo = btrfdll.getFieldTmlBlock(fieldInfo, 1)
subBlock = btrfdll.createBtrfBlock(subBlockInfo, rootBlock)

#Add 16 float values (the array size of 16 is known by the tml file, if you try to write a float at index 16, it will complain about the index being too large)
btrfdll.setDataFloatBtrfBlock(subBlock, 0, c_float(0.1))
btrfdll.setDataFloatBtrfBlock(subBlock, 1, c_float(0.2))
btrfdll.setDataFloatBtrfBlock(subBlock, 2, c_float(0.3))
btrfdll.setDataFloatBtrfBlock(subBlock, 3, c_float(0.4))
btrfdll.setDataFloatBtrfBlock(subBlock, 4, c_float(0.5))
btrfdll.setDataFloatBtrfBlock(subBlock, 5, c_float(0.6))
btrfdll.setDataFloatBtrfBlock(subBlock, 6, c_float(0.7))
btrfdll.setDataFloatBtrfBlock(subBlock, 7, c_float(0.8))
btrfdll.setDataFloatBtrfBlock(subBlock, 8, c_float(0.9))
btrfdll.setDataFloatBtrfBlock(subBlock, 9, c_float(1.0))
btrfdll.setDataFloatBtrfBlock(subBlock, 10, c_float(1.1))
btrfdll.setDataFloatBtrfBlock(subBlock, 11, c_float(1.2))
btrfdll.setDataFloatBtrfBlock(subBlock, 12, c_float(1.3))
btrfdll.setDataFloatBtrfBlock(subBlock, 13, c_float(1.4))
btrfdll.setDataFloatBtrfBlock(subBlock, 14, c_float(1.5))
btrfdll.setDataFloatBtrfBlock(subBlock, 15, c_float(1.6))

#add the float array subblock to the template block
btrfdll.addBlockBtrfBlock(block, subBlock)

#add the block to the rootblock
btrfdll.addBlockBtrfRootBlock(rootBlock, block)

#output to the console the content of the BTRF DOM with just created
btrfdll.dumpToStdoutBtrfRootBlock(rootBlock)

#write it to a file
btrfwriter = btrfdll.createBtrfParser(tmlFile)
btrfdll.writeFileBtrfParser(btrfwriter, b"test.btrf", rootBlock)
