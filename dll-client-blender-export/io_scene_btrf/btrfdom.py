#!/usr/bin/kate
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

from ctypes import c_bool, c_char, c_short, c_int, c_float, c_char_p, c_void_p
import ctypes
import os
import sys
import locale

script_dir = os.path.dirname(os.path.abspath(__file__))
if sys.platform == 'win32':
	from ctypes import windll
	btrfdll = windll.LoadLibrary(script_dir + "/BTRFdom.dll")
else:
	from ctypes import cdll
	btrfdll = cdll.LoadLibrary(script_dir + "/BTRFdom.so")

btrfdll.deleteObject.argtypes = [c_void_p]
btrfdll.deleteObject.restype = None

btrfdll.createBtrfBlock.argtypes = [c_void_p, c_void_p]
btrfdll.createBtrfBlock.restype = c_void_p
btrfdll.setElementNumberBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.setElementNumberBtrfBlock.restype = None

btrfdll.setTemplateIdBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.setTemplateIdBtrfBlock.restype = None
btrfdll.getFieldInfoBtrfBlock.argtypes = [c_void_p]
btrfdll.getFieldInfoBtrfBlock.restype = c_void_p
btrfdll.getTypeBtrfBlock.argtypes = [c_void_p]
btrfdll.getTypeBtrfBlock.restype = c_int
btrfdll.getNameBtrfBlock.argtypes = [c_void_p]
btrfdll.getNameBtrfBlock.restype = c_char_p
btrfdll.getElementNumberBtrfBlock.argtypes = [c_void_p]
btrfdll.getElementNumberBtrfBlock.restype = c_int
btrfdll.getTemplateIdBtrfBlock.argtypes = [c_void_p]
btrfdll.getTemplateIdBtrfBlock.restype = c_int
btrfdll.getTemplateGuidBtrfBlock.argtypes = [c_void_p]
btrfdll.getTemplateGuidBtrfBlock.restype = c_char_p

btrfdll.addBlockBtrfBlock.argtypes = [c_void_p, c_void_p]
btrfdll.addBlockBtrfBlock.restype = None
btrfdll.getBlockBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.getBlockBtrfBlock.restype = c_void_p

btrfdll.setDataCharBtrfBlock.argtypes = [c_void_p, c_int, c_char]
btrfdll.setDataCharBtrfBlock.restype = None
btrfdll.setDataShortBtrfBlock.argtypes = [c_void_p, c_int, c_short]
btrfdll.setDataShortBtrfBlock.restype = None
btrfdll.setDataIntBtrfBlock.argtypes = [c_void_p, c_int, c_int]
btrfdll.setDataIntBtrfBlock.restype = None
btrfdll.setDataFloatBtrfBlock.argtypes = [c_void_p, c_int, c_float]
btrfdll.setDataFloatBtrfBlock.restype = None
btrfdll.setDataStringIdBtrfBlock.argtypes = [c_void_p, c_int, c_int]
btrfdll.setDataStringIdBtrfBlock.restype = None
btrfdll.setDataStringBtrfBlock.argtypes = [c_void_p, c_int, c_char_p]
btrfdll.setDataStringBtrfBlock.restype = None

btrfdll.setDataCharPtrBtrfBlock.argtypes = [c_void_p, ctypes.POINTER(c_char)]
btrfdll.setDataCharPtrBtrfBlock.restype = None
btrfdll.setDataShortPtrBtrfBlock.argtypes = [c_void_p, ctypes.POINTER(c_short)]
btrfdll.setDataShortPtrBtrfBlock.restype = None
btrfdll.setDataIntPtrBtrfBlock.argtypes = [c_void_p, ctypes.POINTER(c_int)]
btrfdll.setDataIntPtrBtrfBlock.restype = None
btrfdll.setDataFloatPtrBtrfBlock.argtypes = [c_void_p, ctypes.POINTER(c_float)]
btrfdll.setDataFloatPtrBtrfBlock.restype = None
btrfdll.setDataStringIdPtrBtrfBlock.argtypes = [c_void_p, ctypes.POINTER(c_int)]
btrfdll.setDataStringIdPtrBtrfBlock.restype = None

btrfdll.getDataCharBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.getDataCharBtrfBlock.restype = c_char
btrfdll.getDataShortBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.getDataShortBtrfBlock.restype = c_short
btrfdll.getDataIntBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.getDataIntBtrfBlock.restype = c_int
btrfdll.getDataFloatBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.getDataFloatBtrfBlock.restype = c_float
btrfdll.getDataStringBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.getDataStringBtrfBlock.restype = c_char_p
btrfdll.getDataStringIdBtrfBlock.argtypes = [c_void_p, c_int]
btrfdll.getDataStringIdBtrfBlock.restype = c_int

btrfdll.getDataCharPtrBtrfBlock.argtypes = [c_void_p]
btrfdll.getDataCharPtrBtrfBlock.restype = ctypes.POINTER(c_char)
btrfdll.getDataShortPtrBtrfBlock.argtypes = [c_void_p]
btrfdll.getDataShortPtrBtrfBlock.restype = ctypes.POINTER(c_short)
btrfdll.getDataIntPtrBtrfBlock.argtypes = [c_void_p]
btrfdll.getDataIntPtrBtrfBlock.restype = ctypes.POINTER(c_int)
btrfdll.getDataFloatPtrBtrfBlock.argtypes = [c_void_p]
btrfdll.getDataFloatPtrBtrfBlock.restype = ctypes.POINTER(c_float)
btrfdll.getDataStringIdPtrBtrfBlock.argtypes = [c_void_p]
btrfdll.getDataStringIdPtrBtrfBlock.restype = ctypes.POINTER(c_int)

btrfdll.createBtrfParser.argtypes = [c_void_p]
btrfdll.createBtrfParser.restype = c_void_p

btrfdll.readFileBtrfParser.argtypes = [c_void_p, c_char_p]
btrfdll.readFileBtrfParser.restype = c_void_p
btrfdll.writeFileBtrfParser.argtypes = [c_void_p, c_char_p, c_void_p]
btrfdll.writeFileBtrfParser.restype = None

btrfdll.createBtrfRootBlock.argtypes = [c_void_p]
btrfdll.createBtrfRootBlock.restype = c_void_p

btrfdll.addStringBtrfRootBlock.argtypes = [c_void_p, c_char_p]
btrfdll.addStringBtrfRootBlock.restype = c_int
btrfdll.getStringBtrfRootBlock.argtypes = [c_void_p, c_int]
btrfdll.getStringBtrfRootBlock.restype = c_char_p

btrfdll.addTemplateBtrfRootBlock.argtypes = [c_void_p, c_char_p, c_int]
btrfdll.addTemplateBtrfRootBlock.restype = c_int
btrfdll.getTemplateGuidBtrfRootBlock.argtypes = [c_void_p, c_int]
btrfdll.getTemplateGuidBtrfRootBlock.restype = c_char_p
btrfdll.getTemplateUsedFieldBtrfRootBlock.argtypes = [c_void_p, c_int]
btrfdll.getTemplateUsedFieldBtrfRootBlock.restype = c_int
btrfdll.getStringNumBtrfRootBlock.argtypes = [c_void_p]
btrfdll.getStringNumBtrfRootBlock.restype = c_int
btrfdll.getTemplateNumBtrfRootBlock.argtypes = [c_void_p]
btrfdll.getTemplateNumBtrfRootBlock.restype = c_int

btrfdll.getTmlFileBtrfRootBlock.argtypes = [c_void_p]
btrfdll.getTmlFileBtrfRootBlock.restype = c_void_p

btrfdll.addBlockBtrfRootBlock.argtypes = [c_void_p, c_void_p]
btrfdll.addBlockBtrfRootBlock.restype = c_int
btrfdll.getBlockByGuidBtrfRootBlock.argtypes = [c_void_p, c_char_p]
btrfdll.getBlockByGuidBtrfRootBlock.restype = c_void_p
btrfdll.getBlockByNameBtrfRootBlock.argtypes = [c_void_p, c_int]
btrfdll.getBlockByNameBtrfRootBlock.restype = c_void_p
btrfdll.getBlockNumBtrfRootBlock.argtypes = [c_void_p]
btrfdll.getBlockNumBtrfRootBlock.restype = c_int

btrfdll.dumpToStdoutBtrfRootBlock.argtypes = [c_void_p]
btrfdll.dumpToStdoutBtrfRootBlock.restype = None

btrfdll.createTmlBlock.argtypes = []
btrfdll.createTmlBlock.restype = c_void_p

btrfdll.getTemplateGuidTmlBlock.argtypes = [c_void_p]
btrfdll.getTemplateGuidTmlBlock.restype = c_char_p
btrfdll.getNameTmlBlock.argtypes = [c_void_p]
btrfdll.getNameTmlBlock.restype = c_char_p
btrfdll.getFieldTmlBlock.argtypes = [c_void_p, c_int]
btrfdll.getFieldTmlBlock.restype = c_void_p
btrfdll.getFieldCountTmlBlock.argtypes = [c_void_p]
btrfdll.getFieldCountTmlBlock.restype = c_int
btrfdll.setFieldCountTmlBlock.argtypes = [c_void_p, c_int]
btrfdll.setFieldCountTmlBlock.restype = None
btrfdll.getTypeTmlBlock.argtypes = [c_void_p]
btrfdll.getTypeTmlBlock.restype = c_int
btrfdll.getHasVariableSizeTmlBlock.argtypes = [c_void_p]
btrfdll.getHasVariableSizeTmlBlock.restype = c_bool

btrfdll.createTmlFile.argtypes = []
btrfdll.createTmlFile.restype = c_void_p

btrfdll.parseFileTmlFile.argtypes = [c_void_p, c_char_p]
btrfdll.parseFileTmlFile.restype = c_bool
btrfdll.getTemplateByGuidTmlFile.argtypes = [c_void_p, c_char_p]
btrfdll.getTemplateByGuidTmlFile.restype = c_void_p
btrfdll.getTemplateByNameTmlFile.argtypes = [c_void_p, c_char_p]
btrfdll.getTemplateByNameTmlFile.restype = c_void_p
btrfdll.addTemplateTmlFile.argtypes = [c_void_p, c_void_p]
btrfdll.addTemplateTmlFile.restype = None


def wrap_ptr(classname, ptr):
	if not ptr or ptr == 0:
		return None
	else:
		return classname(ptr)


# This type is used to prevent function call with a null pointer
class NullPointer:
	__slots__ = ()


class Object:
	__slots__ = ("internal_object")

	def __init__(self, init=NullPointer()):
		if(not init or init == 0):
			self.internal_object = NullPointer()
		else:
			self.internal_object = init

	def delete(self):
		btrfdll.deleteObject(self.internal_object)
		self.internal_object = NullPointer()


class BtrfBlock(Object):
	__slots__ = ()

	def __init__(self, init=NullPointer()):
		Object.__init__(self, init)

	def create(self, fieldInfo, rootBlock):
		self.internal_object = btrfdll.createBtrfBlock(fieldInfo.internal_object, rootBlock.internal_object)

	def setElementNumber(self, count):
		return btrfdll.setElementNumberBtrfBlock(self.internal_object, count)

	def setTemplateId(self, c_int):
		return btrfdll.setTemplateIdBtrfBlock(self.internal_object, c_int)

	def getFieldInfo(self):
		return wrap_ptr(TmlBlock, btrfdll.getFieldInfoBtrfBlock(self.internal_object))

	def getType(self):
		return btrfdll.getTypeBtrfBlock(self.internal_object)

	def getName(self):
		val = btrfdll.getNameBtrfBlock(self.internal_object)
		if not val:
			return "(null)"
		else:
			return val.decode(locale.getdefaultlocale()[1], "replace")

	def getElementNumber(self):
		return btrfdll.getElementNumberBtrfBlock(self.internal_object)

	def getTemplateId(self):
		return btrfdll.getTemplateIdBtrfBlock(self.internal_object)

	def getTemplateGuid(self):
		return btrfdll.getTemplateGuidBtrfBlock(self.internal_object)

	def addBlock(self, other):
		return btrfdll.addBlockBtrfBlock(self.internal_object, other.internal_object)

	def getBlock(self, c_int):
		return wrap_ptr(BtrfBlock, btrfdll.getBlockBtrfBlock(self.internal_object, c_int))

	def setDataChar(self, c_int, c_char):
		return btrfdll.setDataCharBtrfBlock(self.internal_object, c_int, c_char)

	def setDataShort(self, c_int, c_short):
		return btrfdll.setDataShortBtrfBlock(self.internal_object, c_int, c_short)

	def setDataInt(self, c_int, c_int2):
		return btrfdll.setDataIntBtrfBlock(self.internal_object, c_int, c_int2)

	def setDataFloat(self, c_int, c_float):
		return btrfdll.setDataFloatBtrfBlock(self.internal_object, c_int, c_float)

	def setDataStringId(self, c_int, c_int2):
		return btrfdll.setDataStringIdBtrfBlock(self.internal_object, c_int, c_int2)

	def setDataString(self, c_int, c_char_p):
		return btrfdll.setDataStringBtrfBlock(self.internal_object, c_int, c_char_p.encode(locale.getdefaultlocale()[1], "replace"))

	def setDataCharPtr(self, c_char):
		return btrfdll.setDataCharPtrBtrfBlock(self.internal_object, c_char)

	def setDataShortPtr(self, c_short):
		return btrfdll.setDataShortPtrBtrfBlock(self.internal_object, c_short)

	def setDataIntPtr(self, c_int):
		return btrfdll.setDataIntPtrBtrfBlock(self.internal_object, c_int)

	def setDataFloatPtr(self, c_float):
		return btrfdll.setDataFloatPtrBtrfBlock(self.internal_object, c_float)

	def setDataStringIdPtr(self, c_int):
		return btrfdll.setDataStringIdPtrBtrfBlock(self.internal_object, c_int)

	def getDataChar(self, c_int):
		return btrfdll.getDataCharBtrfBlock(self.internal_object, c_int)

	def getDataShort(self, c_int):
		return btrfdll.getDataShortBtrfBlock(self.internal_object, c_int)

	def getDataInt(self, c_int):
		return btrfdll.getDataIntBtrfBlock(self.internal_object, c_int)

	def getDataFloat(self, c_int):
		return btrfdll.getDataFloatBtrfBlock(self.internal_object, c_int)

	def getDataString(self, c_int):
		val = btrfdll.getDataStringBtrfBlock(self.internal_object, c_int)
		if not val:
			return "(null)"
		else:
			return val.decode(locale.getdefaultlocale()[1], "replace")

	def getDataStringId(self, c_int):
		return btrfdll.getDataStringIdBtrfBlock(self.internal_object, c_int)

	def getDataCharPtr(self):
		return btrfdll.getDataCharPtrBtrfBlock(self.internal_object)

	def getDataShortPtr(self):
		return btrfdll.getDataShortPtrBtrfBlock(self.internal_object)

	def getDataIntPtr(self):
		return btrfdll.getDataIntPtrBtrfBlock(self.internal_object)

	def getDataFloatPtr(self):
		return btrfdll.getDataFloatPtrBtrfBlock(self.internal_object)

	def getDataStringIdPtr(self):
		return btrfdll.getDataStringIdPtrBtrfBlock(self.internal_object)


class BtrfParser(Object):
	__slots__ = ()

	def __init__(self, init=NullPointer()):
		Object.__init__(self, init)

	def create(self, tmlFile):
		self.internal_object = btrfdll.createBtrfParser(tmlFile.internal_object)

	def readFile(self, filename):
		return wrap_ptr(BtrfRootBlock, btrfdll.readFileBtrfParser(self.internal_object, filename.encode(locale.getdefaultlocale()[1])))

	def writeFile(self, filename, rootblock):
		return btrfdll.writeFileBtrfParser(self.internal_object, filename.encode(locale.getdefaultlocale()[1]), rootblock.internal_object)


class BtrfRootBlock(Object):
	__slots__ = ()

	def __init__(self, init=NullPointer()):
		Object.__init__(self, init)

	def create(self, tmlFile):
		self.internal_object = btrfdll.createBtrfRootBlock(tmlFile.internal_object)

	def addString(self, c_char_p):
		return btrfdll.addStringBtrfRootBlock(self.internal_object, c_char_p.encode(locale.getdefaultlocale()[1], "replace"))

	def getString(self, c_int):
		val = btrfdll.getStringBtrfRootBlock(self.internal_object, c_int)
		if not val:
			return "(null)"
		else:
			return val.decode(locale.getdefaultlocale()[1], "replace")

	def addTemplate(self, c_char_p, c_int):
		return btrfdll.addTemplateBtrfRootBlock(self.internal_object, c_char_p, c_int)

	def getTemplateGuid(self, c_int):
		return btrfdll.getTemplateGuidBtrfRootBlock(self.internal_object, c_int)

	def getTemplateUsedField(self, c_int):
		return btrfdll.getTemplateUsedFieldBtrfRootBlock(self.internal_object, c_int)

	def getStringNum(self):
		return btrfdll.getStringNumBtrfRootBlock(self.internal_object)

	def getTemplateNum(self):
		return btrfdll.getTemplateNumBtrfRootBlock(self.internal_object)

	def getTmlFile(self):
		return wrap_ptr(TmlFile, btrfdll.getTmlFileBtrfRootBlock(self.internal_object))

	def addBlock(self, block):
		return btrfdll.addBlockBtrfRootBlock(self.internal_object, block.internal_object)

	def getBlockByGuid(self, c_char_p):
		return wrap_ptr(BtrfBlock, btrfdll.getBlockByGuidBtrfRootBlock(self.internal_object, c_char_p))

	def getBlockByName(self, c_int):
		return wrap_ptr(BtrfBlock, btrfdll.getBlockByNameBtrfRootBlock(self.internal_object, c_int))

	def getBlockNum(self):
		return btrfdll.getBlockNumBtrfRootBlock(self.internal_object)

	def dumpToStdout(self):
		return btrfdll.dumpToStdoutBtrfRootBlock(self.internal_object)


class TmlBlock(Object):
	__slots__ = ()

	def __init__(self, init=NullPointer()):
		Object.__init__(self, init)

	def create(self):
		self.internal_object = btrfdll.createTmlBlock()

	def getTemplateGuid(self):
		return btrfdll.getTemplateGuidTmlBlock(self.internal_object)

	def getName(self):
		val = btrfdll.getNameTmlBlock(self.internal_object)
		if not val:
			return "(null)"
		else:
			return val.decode(locale.getdefaultlocale()[1], "replace")

	def getField(self, c_int):
		return wrap_ptr(TmlBlock, btrfdll.getFieldTmlBlock(self.internal_object, c_int))

	def getFieldCount(self):
		return btrfdll.getFieldCountTmlBlock(self.internal_object)

	def setFieldCount(self, c_int):
		return btrfdll.setFieldCountTmlBlock(self.internal_object, c_int)

	def getType(self):
		return btrfdll.getTypeTmlBlock(self.internal_object)

	def getHasVariableSize(self):
		return btrfdll.getHasVariableSizeTmlBlock(self.internal_object)


class TmlFile(Object):
	__slots__ = ()

	def __init__(self, init=NullPointer()):
		Object.__init__(self, init)

	def create(self):
		self.internal_object = btrfdll.createTmlFile()

	def parseFile(self, c_char_p):
		return btrfdll.parseFileTmlFile(self.internal_object, c_char_p.encode(locale.getdefaultlocale()[1]))

	def getTemplateByGuid(self, c_char_p):
		return wrap_ptr(TmlBlock, btrfdll.getTemplateByGuidTmlFile(self.internal_object, c_char_p))

	def getTemplateByName(self, c_char_p):
		return wrap_ptr(TmlBlock, btrfdll.getTemplateByNameTmlFile(self.internal_object, c_char_p.encode(locale.getdefaultlocale()[1], "replace")))

	def addTemplate(self, c_void_p2):
		return btrfdll.addTemplateTmlFile(self.internal_object, c_void_p2)
