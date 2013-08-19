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
from ctypes import windll, c_float, c_int
import uuid
import mathutils
import os

nx3_version_header_guid = uuid.UUID('{81BCE021-AD76-346f-9C7D-19885FD118B6}')

nx3_mtl_header_guid = uuid.UUID('{209BBB41-681F-4b9b-9744-4D88E1413DCC}')
nx3_mtl_guid = uuid.UUID('{52BCCAA6-3C16-4286-8B9E-1A798F9D94DE}')
nx3_mtl_block_guid = uuid.UUID('{81BCE071-AC76-496f-9C7D-19885FD118B6}')

nx3_new_mesh_header_guid = uuid.UUID('{A6D25AEB-A735-1fef-C17D-EE2117498226}')
nx3_new_mesh_guid = uuid.UUID('{1718DC1B-1DB1-458a-9C7E-C3D46FC4585B}')
nx3_mesh_block_guid = uuid.UUID('{C817F7B0-E4E7-40fb-97B3-2B97CC000521}')
nx3_mesh_frame_guid = uuid.UUID('{1C77954B-CDD5-4615-B7AD-F23BD3D0C23E}')
nx3_weight_frame_guid = uuid.UUID('{B513DF30-80BE-44f4-980B-84B9D979A607}')
nx3_mesh_tm_guid = uuid.UUID('{F09C560E-7328-411e-87A3-EEB165D5F929}')

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
template nx3_new_mesh_header {
	dword			mesh_size;
	nx3_new_mesh    mesh_array[mesh_size] {	
		string			mesh_name;

		dword			material_id;
		dword			channel_id;

		dword			mesh_block_size;
		nx3_mesh_block	mesh_block_array[mesh_block_size] {
			dword			texture_index;	
			
			dword			mesh_frame_size;
			nx3_mesh_frame	mesh_frame_array[mesh_frame_size] {
				dword   time_value;

				dword	vertex_size;
				float	vertex_array[vertex_size];
				
				dword	normal_size;
				float	normal_array[normal_size];

				dword	texel_size;
				float	texel_array[texel_size];
				
				dword	color_size;
				dword	color_array[color_size];

				dword				bone_size;
				nx3_weight_frame	bone_block[bone_size] {
					string  bone_name;
					dword	weight_size;
					float	weight_array[weight_size];
					dword	offset_vector_size;
					float   offset_vector_array[offset_vector_size];	
				}
				
				float	mesh_tm[16];	
			}

			dword			index_buffer_size;
			word			index_buffer_array[index_buffer_size];
			
			//Not used
			dword			lightmap_index;
		}
		
		dword			ani_time_size;
		dword			ani_time_array[ani_time_size];
		dword			ani_matrix_size;
		float			ani_matrix_array[ani_matrix_size];
		
		dword			visi_time_size;
		dword			visi_time_array[visi_time_size];
		float			visi_value_array[visi_time_size];
			
		dword			fx_size;
		nx3_fx			fx_array[fx_size];

		dword			mesh_children_size;
		nx3_new_mesh    mesh_children_array[mesh_children_size];
	}
	dword			mesh_tm_size;
	nx3_mesh_tm		mesh_tm_array[mesh_tm_size] {
		string		name;
		float       tm[16];
	}
}
"""
#bones: bpy.data.objects['Cube'].modifiers['Armature'].object.data.bones[...]
#vertex_group: bpy.data.objects['Cube'].vertex_groups
#vertices_num: len(bpy.data.objects['Cube'].data.vertices)
#faces: bpy.data.objects['Cube'].data.polygons[...]
#normals: bpy.data.objects['Cube'].data.polygons[0].normal
#vertices: bpy.data.objects['Cube'].data.vertices[...].co
#transform matrix: bpy.data.objects['Cube'].matrix_world
#texture: bpy.data.objects['mechanic_butkadah3.0'].data.uv_textures[0].data[0].image.filepath
#texture uv coords: bpy.data.objects['mechanic_butkadah3.0'].data.uv_layers['UVMap'].data[0].uv
#texture_index: bpy.data.materials.find(bpy.data.objects['mechanic_butkadah3.0'].data.materials[0].name)
#face indices: bpy.data.objects['Cube'].data.polygons[0].vertices[0..2]

def get_ascii_str(input):
	if(input == None):
		return b"(null)"
	else:
		return input.encode('ascii')

def get_texture_filename(material):
	try:
		if material.texture_slots:
			texture_slot = material.texture_slots[0]
			if texture_slot != None:
				texture = texture_slot.texture
				if texture != None and texture.type == 'IMAGE':
					return get_ascii_str(os.path.basename(texture.image.filepath))
	except:
		pass
	return b"(null)"
	

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

#version
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
def get_mtl_block(btrfdll, tmlFile, rootBlock, material, id):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_mtl_block_guid.bytes_le)
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

def get_mtl_data(btrfdll, tmlFile, rootBlock):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_mtl_guid.bytes_le)
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)

	arrayBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	#iterate over all used materials
	for object in bpy.data.objects:
		if object.type == 'MESH' and object.data.materials[0]:
			mat = object.data.materials[0]
			subBlock = get_mtl_block(btrfdll, tmlFile, rootBlock, mat, bpy.data.materials.find(mat.name))
			btrfdll.addBlockBtrfBlock(arrayBlock, subBlock)
	
	btrfdll.addBlockBtrfBlock(block, arrayBlock)
	return block

def write_mtl_header(btrfdll, tmlFile, rootBlock):
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_mtl_header_guid.bytes_le)
	
	#Create a block that will contain the data of the template
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)

	arrayBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	subBlock = get_mtl_data(btrfdll, tmlFile, rootBlock)
	btrfdll.addBlockBtrfBlock(arrayBlock, subBlock)
	btrfdll.addBlockBtrfBlock(block, arrayBlock)
	
	btrfdll.addBlockBtrfRootBlock(rootBlock, block)

#Meshes data
def get_vertex_index_weight(vertex_group, vertices_num):
	data = []
	for index in range(vertices_num):
		try:
			weight = vertex_group.weight(index)
			data.append(c_float(index))
			data.append(c_float(weight))
		except RuntimeError:
			pass

	return data

def get_nx3_weight_frame(btrfdll, tmlFile, rootBlock, vertex_group, vertices_num):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_weight_frame_guid.bytes_le)
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	weight_list = get_vertex_index_weight(vertex_group, vertices_num)
	
	bone_name = get_ascii_str(vertex_group.name)
	weight_size = len(weight_list)
	weight_array = weight_list
	offset_vector_size = int(len(weight_list)/2*3)
	#offset_vector_array = [0 for i in range(offset_vector_size)]

	#string  bone_name
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	btrfdll.setDataStringBtrfBlock(subBlock, 0, bone_name)
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#float  weight_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 1), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, weight_size)
	for i in range(weight_size):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, weight_array[i])
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#float  offset_vector_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 2), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, offset_vector_size)
	for i in range(offset_vector_size):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, 0)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	return block

def get_nx3_mesh_frame(btrfdll, tmlFile, rootBlock, object, mesh, vertex_groups):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_mesh_frame_guid.bytes_le)
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)

	vertices_num = len(mesh.vertices)
	
	time_value = 0
	vertex_array = [ coord for face in mesh.polygons for vertexId in face.vertices for coord in mesh.vertices[vertexId].co]
	normal_array = [ (coord) for face in mesh.polygons for vertexId in face.vertices for coord in mesh.vertices[vertexId].normal]
	if len(mesh.tessface_uv_textures) > 0:
		texel_array = [ (uv[0], 1-uv[1]) for uvface in mesh.tessface_uv_textures[0].data for uv in uvface.uv ]
	else:
		texel_array = [[]]
	color_array = []
	
	bone_block = [ get_nx3_weight_frame(btrfdll, tmlFile, rootBlock, vertex_group, vertices_num) for vertex_group in vertex_groups]
	mesh_tm = [val for vect in object.matrix_world.transposed() for val in vect]

	#dword  time_value
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, time_value)
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#float  vertex_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 1), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(vertex_array))
	for i in range(len(vertex_array)):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, c_float(vertex_array[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#float  normal_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 2), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(normal_array))
	for i in range(len(normal_array)):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, c_float(normal_array[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#float  texel_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 3), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, int(len(texel_array)*2))
	for i in range(len(texel_array)):
		btrfdll.setDataFloatBtrfBlock(subBlock, int(i*2), c_float(texel_array[i][0]))
		btrfdll.setDataFloatBtrfBlock(subBlock, int(i*2+1), c_float(texel_array[i][1]))
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#dword  color_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 4), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(color_array))
	for i in range(len(color_array)):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, c_int(color_array[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#nx3_weight_frame  bone_block[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 5), rootBlock)
	for bone in bone_block:
		btrfdll.addBlockBtrfBlock(subBlock, bone)
	btrfdll.addBlockBtrfBlock(block, subBlock)

	#float  mesh_tm[16]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 6), rootBlock)
	for i in range(16):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, c_float(mesh_tm[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	return block

def get_nx3_mesh_block(btrfdll, tmlFile, rootBlock, object):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_mesh_block_guid.bytes_le)
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	mesh = object.to_mesh(bpy.context.scene, True, 'PREVIEW')
	vertex_groups = object.vertex_groups
	
	if len(mesh.materials) > 0:
		texture_index = bpy.data.materials.find(mesh.materials[0].name)
	else:
		texture_index = 0
	mesh_block = get_nx3_mesh_frame(btrfdll, tmlFile, rootBlock, object, mesh, vertex_groups)
	index_buffer_array = [ (i*3+2, i*3+1, i*3) for i in range(len(mesh.polygons)) ]
	index_buffer_array = [ index for face in index_buffer_array for index in face ]

	#dword  texture_index
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, texture_index)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#nx3_mesh_frame  mesh_frame_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 1), rootBlock)
	btrfdll.addBlockBtrfBlock(subBlock, mesh_block)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#word  index_buffer_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 2), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(index_buffer_array))
	for i in range(len(index_buffer_array)):
		btrfdll.setDataShortBtrfBlock(subBlock, i, index_buffer_array[i])
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	return block
	
def get_nx3_new_mesh(btrfdll, tmlFile, rootBlock, global_object):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_new_mesh_guid.bytes_le)
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	mesh_name = get_ascii_str(global_object.name)
	material_id = 0
	channel_id = 0

	if global_object.type == 'ARMATURE':
		mesh_block_array = [get_nx3_mesh_block(btrfdll, tmlFile, rootBlock, object) for object in global_object.children if object.type == 'MESH']
	elif global_object.type == 'MESH':
		mesh_block_array = [get_nx3_mesh_block(btrfdll, tmlFile, rootBlock, global_object)]
	else:
		print("No mesh to export !")
		btrfdll.deleteObject(block)
		return None

	ani_time_array = []
	ani_matrix_array = []
	visi_time_array = []
	visi_value_array = []
	fx_array = []
	mesh_children_array = []

	#string  mesh_name
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	btrfdll.setDataStringBtrfBlock(subBlock, 0, mesh_name)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword  material_id
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 1), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, material_id)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword  channel_id
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 2), rootBlock)
	btrfdll.setDataIntBtrfBlock(subBlock, 0, channel_id)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#nx3_mesh_block  mesh_block_array
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 3), rootBlock)
	for i in range(len(mesh_block_array)):
		btrfdll.addBlockBtrfBlock(subBlock, mesh_block_array[i])
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword ani_time_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 4), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(ani_time_array))
	for i in range(len(ani_time_array)):
		btrfdll.setDataIntBtrfBlock(subBlock, i, c_int(ani_time_array[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#float ani_matrix_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 5), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(ani_matrix_array))
	for i in range(len(ani_matrix_array)):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, c_float(ani_matrix_array[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#dword visi_time_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 6), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(visi_time_array))
	for i in range(len(visi_time_array)):
		btrfdll.setDataIntBtrfBlock(subBlock, i, c_int(visi_time_array[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#float visi_value_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 7), rootBlock)
	btrfdll.setElementNumberBtrfBlock(subBlock, len(visi_value_array))
	for i in range(len(visi_value_array)):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, c_float(visi_value_array[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#nx3_fx fx_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 8), rootBlock)
	#unsupported, nothing to add
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#nx3_new_mesh mesh_children_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 9), rootBlock)
	#unsupported, nothing to add
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	return block

def get_nx3_mesh_tm(btrfdll, tmlFile, rootBlock, bone):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_mesh_tm_guid.bytes_le)
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	name = get_ascii_str(bone.name)

	bone_global_matrix = (bone.id_data.matrix_world.inverted()*bone.matrix).inverted().transposed()
	tm = [val for vect in bone_global_matrix for val in vect]

	#string  name
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	btrfdll.setDataStringBtrfBlock(subBlock, 0, name)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#float  tm[16]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 1), rootBlock)
	for i in range(16):
		btrfdll.setDataFloatBtrfBlock(subBlock, i, c_float(tm[i]))
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	return block

def write_nx3_new_mesh_header(btrfdll, tmlFile, rootBlock):
	#Create a block that will contain the data of the template
	fieldInfo = btrfdll.getTemplateByGuidTmlFile(tmlFile, nx3_new_mesh_header_guid.bytes_le)
	block = btrfdll.createBtrfBlock(fieldInfo, rootBlock)
	
	objects = [object for object in bpy.data.objects if object.type == 'ARMATURE']
	
	if len(objects) == 0:
		objects = [object for object in bpy.data.objects if object.type == 'MESH']
	
	mesh_array = [get_nx3_new_mesh(btrfdll, tmlFile, rootBlock, object) for object in objects]
	mesh_tm_array = [get_nx3_mesh_tm(btrfdll, tmlFile, rootBlock, bone) for armature in bpy.data.objects if armature.type == 'ARMATURE' for bone in armature.pose.bones]

	#nx3_new_mesh mesh_array[]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 0), rootBlock)
	for mesh in mesh_array:
		btrfdll.addBlockBtrfBlock(subBlock, mesh)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	#float  tm[16]
	subBlock = btrfdll.createBtrfBlock(btrfdll.getFieldTmlBlock(fieldInfo, 1), rootBlock)
	for mesh_tm in mesh_tm_array:
		btrfdll.addBlockBtrfBlock(subBlock, mesh_tm)
	btrfdll.addBlockBtrfBlock(block, subBlock)
	
	btrfdll.addBlockBtrfRootBlock(rootBlock, block)


def write(filename):
	(btrfdll, tmlFile, rootBlock) = load_btrfdom()

	write_version(btrfdll, tmlFile, rootBlock)
	write_mtl_header(btrfdll, tmlFile, rootBlock)
	write_nx3_new_mesh_header(btrfdll, tmlFile, rootBlock)
	
	writter = btrfdll.createBtrfParser(tmlFile)
	btrfdll.writeFileBtrfParser(writter, filename, rootBlock)
	#btrfdll.dumpToStdoutBtrfRootBlock(rootBlock)