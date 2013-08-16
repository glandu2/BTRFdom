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

import bpy
from ctypes import windll, c_float
import uuid

nx3_version_header_guid = uuid.UUID('{81BCE021-AD76-346f-9C7D-19885FD118B6}')

nx3_mtl_header_guid = uuid.UUID('{209BBB41-681F-4b9b-9744-4D88E1413DCC}')
nx3_mtl_guid = uuid.UUID('{52BCCAA6-3C16-4286-8B9E-1A798F9D94DE}')
nx3_mtl_block_guid = uuid.UUID('{81BCE071-AC76-496f-9C7D-19885FD118B6}')

nx3_new_mesh_header_guid = uuid.UUID('{A6D25AEB-A735-1fef-C17D-EE2117498226}')
nx3_new_mesh_guid = uuid.UUID('{1718DC1B-1DB1-458a-9C7E-C3D46FC4585B}')
nx3_mesh_block_guid = uuid.UUID('{C817F7B0-E4E7-40fb-97B3-2B97CC000521}')
nx3_mesh_frame_guid = uuid.UUID('{1C77954B-CDD5-4615-B7AD-F23BD3D0C23E}')
nx3_weight_frame_guid = uuid.UUID('{B513DF30-80BE-44f4-980B-84B9D979A607}')

"""
NX3 structure:
template nx3_version_header {
	dword	version;
}
template nx3_mtl_header {
	dword	mtl_size;
	nx3_mtl	mtl_array[mtl_size] {
		dword			sub_mtl_block_size;
		nx3_mtl_block	sub_mtl_block_array[sub_mtl_block_size] {
			string  mtl_name;
			string	texture_name;

			dword	mtl_id;
			dword	channel_id;
			
			float	power;	
			float	self_illumi;	
			
			char	smoothing;
				
			dword	ambient;
			dword	diffuse;
			dword	specular;
		}
	}
}
"""

def get_ascii_str(input):
	if(input == None):
		return b"<null>"
	else:
		return input.encode('ascii')

def get_texture_filename(material):
	if material.texture_slots:
		texture_slot = material.texture_slots[0]
		if texture_slot != None:
			texture = texture_slot.texture
			if texture != None and texture.type == 'IMAGE':
				return get_ascii_str(texture.image.filepath)
	return b"<null>"
	

def load_btrfdom():
	#Load the BTRFdom dll (or so on linux)
	btrfdll = windll.LoadLibrary("./BTRFdom")

	#Create a TmlFile object that will contain all known templates and their fields
	tmlFile = btrfdll.createTmlFile()

	#Read some template files
	btrfdll.parseFileTmlFile(tmlFile, b"nx3.tml")
	btrfdll.parseFileTmlFile(tmlFile, b"nobj.tml")
	
	#Create a root block, it will contain all block in the btrf file (it does not exist in the file, it's here only to represent the file with all its blocks)
	rootBlock = btrfdll.createBtrfRootBlock(tmlFile)
	
	return (btrfdll, tmlFile, rootBlock)

def write_version(btrfdll, tmlFile, rootBlock):
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_version_header_guid.bytes_le)
	
	#Create a block that will contain the data of the template
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)

	#dword version
	subBlockInfo = btrfdll.getFieldTmlBlock(fieldInfo, 0)
	subBlock = btrfdll.createBtrfBlock(subBlockInfo, rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, 65536)

	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	btrfdll.addBlockBtrfRootBlock(rootBlock, block)


#Create nx3_mtl_block
def get_mtl_block(btrfdll, fieldInfo, rootBlock, material, id):
	#Create a block that will contain the data of the template
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	mtl_name = get_ascii_str(material.name)
	texture_name = get_texture_filename(material)
	mtl_id = id
	channel_id = 0
	power = c_float(0)
	self_illumi = c_float(0)
	smoothing = 0
	ambient = 0
	diffuse = 0
	specular = 0

	#string  mtl_name
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	btrfdll.setDataStringBtrfBlock(subBlock, 0, mtl_name)
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#string  texture_name
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 1), rootBlock)
	btrfdll.setDataStringBtrfBlock(subBlock, 0, texture_name)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword  mtl_id
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 2), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, mtl_id)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword  channel_id
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 3), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, channel_id)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#float  power
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 4), rootBlock)
	btrfdll.setDataFloatBtrfBlock(subBlock, 0, power)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#float  self_illumi
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 5), rootBlock)
	btrfdll.setDataFloatBtrfBlock(subBlock, 0, self_illumi)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#char  smoothing
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 6), rootBlock)
	btrfdll.setDataCharBtrfBlock(subBlock, 0, smoothing)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword  ambient
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 7), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, ambient)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword  diffuse
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 8), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, diffuse)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword  specular
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 9), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, specular)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	return block


def get_mtl_data(btrfdll, fieldInfo, rootBlock):
	#Create a block that will contain the data of the template
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	arrayFieldInfo = btrfdll.getFieldTmlBlock(fieldInfo, 0)
	arrayBlock = btrfdll.createBtrfBlock(arrayFieldInfo, rootBlock)

	subFieldInfo = btrfdll.getFieldTmlBlock(arrayFieldInfo, 0)

	#iterate over all materials
	id = 0
	for mat in bpy.data.materials:
		if mat != None:
			subBlock = get_mtl_block(btrfdll, subFieldInfo, rootBlock, mat, id)
			btrfdll.addBlockBtrfBlock(arrayBlock, subBlock)
			id = id + 1
	
	btrfdll.addBlockBtrfBlock(block, arrayBlock)
	return block

def write_mtl_header(btrfdll, tmlFile, rootBlock):
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_mtl_header_guid.bytes_le)
	
	#Create a block that will contain the data of the template
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	arrayFieldInfo = btrfdll.getFieldTmlBlock(fieldInfo, 0)
	arrayBlock = btrfdll.createBtrfBlock(arrayFieldInfo, rootBlock)
	
	subFieldInfo = btrfdll.getFieldTmlBlock(arrayFieldInfo, 0)
	subBlock = get_mtl_data(btrfdll, subFieldInfo, rootBlock)

	btrfdll.addBlockBtrfBlock(arrayBlock, subBlock)
	btrfdll.addBlockBtrfBlock(block, arrayBlock)
	
	btrfdll.addBlockBtrfRootBlock(rootBlock, block)


def write(filename):
	(btrfdll, tmlFile, rootBlock) = load_btrfdom()

	write_version(btrfdll, tmlFile, rootBlock)
	write_mtl_header(btrfdll, tmlFile, rootBlock)
	
	writter = btrfdll.createBtrfParser(tmlFile)
	btrfdll.writeFileBtrfParser(writter, filename, rootBlock)