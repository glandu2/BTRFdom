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

import bpy
import uuid
from .btrfdom import BtrfParser, TmlFile, BtrfRootBlock, BtrfBlock
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


class Material:
	def __init__(self, texture_id, material_id, channel_id, material):
		self.texture_id = texture_id
		self.material_id = material_id
		self.channel_id = channel_id
		self.material = material

	def equals(self, other):
		return(isinstance(other, self.__class__)
			and self.texture_id == other.texture_id
			and self.material_id == other.material_id
			and self.channel_id == other.channel_id
			and self.material == other.material)

	def __ne__(self, other):
		return not self.__eq__(other)


used_materials = []


def get_str(input):
	if input is None:
		return "(null)"
	else:
		return input


def get_texture_filename(material):
	try:
		if material.texture_slots:
			texture_slot = material.texture_slots[0]
			if texture_slot is not None:
				texture = texture_slot.texture
				if texture is not None and texture.type == 'IMAGE':
					return get_str(os.path.basename(texture.image.filepath))
	except:
		pass
	return "(null)"


def load_btrfdom():
	#Load the BTRFdom dll (or so on linux)
	script_dir = os.path.dirname(os.path.abspath(__file__))

	#Create a TmlFile object that will contain all known templates and their fields
	tmlFile = TmlFile()
	tmlFile.create()

	#Read some template files
	tmlFile.parseFile(script_dir + "/nx3.tml")
	tmlFile.parseFile(script_dir + "/nobj.tml")

	#Create a root block, it will contain all block in the btrf file (it does not exist in the file, it's here only to represent the file with all its blocks)
	rootBlock = BtrfRootBlock()
	rootBlock.create(tmlFile)

	return (tmlFile, rootBlock)


#version
def write_version(tmlFile, rootBlock):
	fieldInfo = tmlFile.getTemplateByGuid(nx3_version_header_guid.bytes_le)

	#Create a block that will contain the data of the template
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	#dword version
	subBlockInfo = fieldInfo.getField(0)
	subBlock = BtrfBlock()
	subBlock.create(subBlockInfo, rootBlock)
	subBlock.setDataInt(0, 65536)

	block.addBlock(subBlock)

	rootBlock.addBlock(block)


#Create nx3_mtl_block
def get_mtl_block(tmlFile, rootBlock, material_info):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mtl_block_guid.bytes_le)

	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	mtl_name = get_str(material_info.material.name)
	texture_name = get_texture_filename(material_info.material)
	mtl_id = material_info.material_id
	channel_id = material_info.channel_id
	power = 0
	self_illumi = 0
	smoothing = 0
	ambient = 0
	diffuse = 0
	specular = 0

	#string  mtl_name
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataString(0, mtl_name)
	block.addBlock(subBlock)

	#string  texture_name
	subBlock.create(fieldInfo.getField(1), rootBlock)
	subBlock.setDataString(0, texture_name)
	block.addBlock(subBlock)

	#dword  mtl_id
	subBlock.create(fieldInfo.getField(2), rootBlock)
	subBlock.setDataInt(0, mtl_id)
	block.addBlock(subBlock)

	#dword  channel_id
	subBlock.create(fieldInfo.getField(3), rootBlock)
	subBlock.setDataInt(0, channel_id)
	block.addBlock(subBlock)

	#float  power
	subBlock.create(fieldInfo.getField(4), rootBlock)
	subBlock.setDataFloat(0, power)
	block.addBlock(subBlock)

	#float  self_illumi
	subBlock.create(fieldInfo.getField(5), rootBlock)
	subBlock.setDataFloat(0, self_illumi)
	block.addBlock(subBlock)

	#char  smoothing
	subBlock.create(fieldInfo.getField(6), rootBlock)
	subBlock.setDataChar(0, smoothing)
	block.addBlock(subBlock)

	#dword  ambient
	subBlock.create(fieldInfo.getField(7), rootBlock)
	subBlock.setDataInt(0, ambient)
	block.addBlock(subBlock)

	#dword  diffuse
	subBlock.create(fieldInfo.getField(8), rootBlock)
	subBlock.setDataInt(0, diffuse)
	block.addBlock(subBlock)

	#dword  specular
	subBlock.create(fieldInfo.getField(9), rootBlock)
	subBlock.setDataInt(0, specular)
	block.addBlock(subBlock)

	return block


def get_mtl_data(tmlFile, rootBlock):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mtl_guid.bytes_le)

	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	arrayBlock = BtrfBlock()
	arrayBlock.create(fieldInfo.getField(0), rootBlock)
	#iterate over all used materials

	for material_info in used_materials:
		subBlock = get_mtl_block(tmlFile, rootBlock, material_info)
		arrayBlock.addBlock(subBlock)

	block.addBlock(arrayBlock)
	return block


def write_mtl_header(tmlFile, rootBlock):
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mtl_header_guid.bytes_le)

	#Create a block that will contain the data of the template
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	arrayBlock = BtrfBlock()
	arrayBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock = get_mtl_data(tmlFile, rootBlock)
	arrayBlock.addBlock(subBlock)
	block.addBlock(arrayBlock)

	rootBlock.addBlock(block)


#Meshes data
def get_vertex_index_weight(vertex_group, vertex_indices):
	data = []
	for rpz_index, index in enumerate(vertex_indices):
		try:
			weight = vertex_group.weight(index)
			data.append(rpz_index)
			data.append(weight)
		except RuntimeError:
			pass

	return data


def get_nx3_weight_frame(tmlFile, rootBlock, vertex_group, vertex_indices):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_weight_frame_guid.bytes_le)
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	weight_list = get_vertex_index_weight(vertex_group, vertex_indices)

	bone_name = get_str(vertex_group.name)
	weight_size = len(weight_list)
	weight_array = weight_list
	offset_vector_size = int(len(weight_list) / 2 * 3)
	#offset_vector_array = [0 for i in range(offset_vector_size)]

	#string  bone_name
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataString(0, bone_name)
	block.addBlock(subBlock)

	#float  weight_array[]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	subBlock.setElementNumber(weight_size)
	for i in range(weight_size):
		subBlock.setDataFloat(i, weight_array[i])
	block.addBlock(subBlock)

	#float  offset_vector_array[]
	subBlock.create(fieldInfo.getField(2), rootBlock)
	subBlock.setElementNumber(offset_vector_size)
	for i in range(offset_vector_size):
		subBlock.setDataFloat(i, 0)
	block.addBlock(subBlock)

	return block


# Represent a vertex with it's normal and UV coords. Vertex, normals and UV coords cannot be separated in Rappelz NX3 format.

class VertexInfo:
	def __init__(self, vertex_index, vertex, normal, texel=[0, 0]):
		self.vertex_index = vertex_index
		self.vertex = vertex
		self.normal = normal
		self.texel = texel

	def equals(self, other):
		return(isinstance(other, self.__class__)
			and abs(self.vertex[0] - other.vertex[0]) < 0.0001
			and abs(self.vertex[1] - other.vertex[1]) < 0.0001
			and abs(self.vertex[2] - other.vertex[2]) < 0.0001
			and abs(self.normal[0] - other.normal[0]) < 0.0001
			and abs(self.normal[1] - other.normal[1]) < 0.0001
			and abs(self.normal[2] - other.normal[2]) < 0.0001
			and abs(self.texel[0] - other.texel[0]) < 0.0001
			and abs(self.texel[1] - other.texel[1]) < 0.0001)

	def __ne__(self, other):
		return not self.__eq__(other)


def index_of_vertex_info(vertex_info_array, vertex_info):
	for i, v in enumerate(vertex_info_array):
		if vertex_info.equals(v):
			return i
	return -1


def get_nx3_mesh_frame(tmlFile, rootBlock, mesh_matrix, vertex_info_array, vertex_groups, has_texel):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mesh_frame_guid.bytes_le)

	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	time_value = 0
	vertex_array = [coord for vertex_info in vertex_info_array for coord in vertex_info.vertex]
	normal_array = [coord for vertex_info in vertex_info_array for coord in vertex_info.normal]

	if has_texel is True:
		texel_array = [(vertex_info.texel[0], 1 - vertex_info.texel[1]) for vertex_info in vertex_info_array]
	else:
		texel_array = []

	color_array = []

	vertex_indices = [vertex_info.vertex_index for vertex_info in vertex_info_array]
	bone_block = [get_nx3_weight_frame(tmlFile, rootBlock, vertex_group, vertex_indices) for vertex_group in vertex_groups]
	mesh_tm = [val for vect in mesh_matrix.transposed() for val in vect]

	#dword  time_value
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataInt(0, time_value)
	block.addBlock(subBlock)

	#float  vertex_array[]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	vertex_array.setElementNumber(subBlock, len())
	for i in range(len(vertex_array)):
		subBlock.setDataFloat(i, vertex_array[i])
	block.addBlock(subBlock)

	#float  normal_array[]
	subBlock.create(fieldInfo.getField(2), rootBlock)
	normal_array.setElementNumber(subBlock, len())
	for i in range(len(normal_array)):
		subBlock.setDataFloat(i, normal_array[i])
	block.addBlock(subBlock)

	#float  texel_array[]
	subBlock.create(fieldInfo.getField(3), rootBlock)
	texel_array.setElementNumber(subBlock, int(len() * 2))
	for i in range(len(texel_array)):
		subBlock.setDataFloat(int(i * 2), texel_array[i][0])
		subBlock.setDataFloat(int(i * 2 + 1), texel_array[i][1])
	block.addBlock(subBlock)

	#dword  color_array[]
	subBlock.create(fieldInfo.getField(4), rootBlock)
	color_array.setElementNumber(subBlock, len())
	for i in range(len(color_array)):
		subBlock.setDataInt(i, color_array[i])
	block.addBlock(subBlock)

	#nx3_weight_frame  bone_block[]
	subBlock.create(fieldInfo.getField(5), rootBlock)
	for bone in bone_block:
		subBlock.addBlock(bone)
	block.addBlock(subBlock)

	#float  mesh_tm[16]
	subBlock.create(fieldInfo.getField(6), rootBlock)
	for i in range(16):
		subBlock.setDataFloat(i, mesh_tm[i])
	block.addBlock(subBlock)

	return block

current_texture_index = 0


def get_nx3_mesh_block(tmlFile, rootBlock, object):
	global used_materials, current_texture_index
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mesh_block_guid.bytes_le)

	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	mesh = object.to_mesh(bpy.context.scene, True, 'PREVIEW')
	vertex_groups = object.vertex_groups
	vertex_info_array = []
	index_array = []

	mesh.update(calc_tessface=True)

	if len(mesh.materials) > 0 and len(mesh.tessface_uv_textures) > 0:
		material_info = Material(current_texture_index, 0, 0, mesh.materials[0])
		texture_index = current_texture_index
		current_texture_index = current_texture_index + 1
		used_materials.append(material_info)
		has_material = True
	else:
		texture_index = 0
		has_material = False

	for face in mesh.tessfaces:
		for i, vertex_index in reversed(list(enumerate(face.vertices))):    # reversed instead of enumerate else normals are wrong
			if has_material is True:
				uv_texture = mesh.tessface_uv_textures[0]
				vertex_info = VertexInfo(vertex_index, mesh.vertices[vertex_index].co, mesh.vertices[vertex_index].normal, uv_texture.data[face.index].uv[i])
			else:
				vertex_info = VertexInfo(vertex_index, mesh.vertices[vertex_index].co, mesh.vertices[vertex_index].normal, [])

			index = index_of_vertex_info(vertex_info_array, vertex_info)
			if index == -1:
				index = len(vertex_info_array)
				vertex_info_array.append(vertex_info)
			index_array.append(index)

	mesh_block = get_nx3_mesh_frame(tmlFile, rootBlock, object.matrix_world, vertex_info_array, vertex_groups, has_material)

	#dword  texture_index
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataInt(0, texture_index)
	block.addBlock(subBlock)

	#nx3_mesh_frame  mesh_frame_array[]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	subBlock.addBlock(mesh_block)
	block.addBlock(subBlock)

	#word  index_buffer_array[]
	subBlock.create(fieldInfo.getField(2), rootBlock)
	index_array.setElementNumber(subBlock, len())
	for i in range(len(index_array)):
		subBlock.setDataShort(i, index_array[i])
	block.addBlock(subBlock)

	bpy.data.meshes.remove(mesh)

	return block


def get_nx3_new_mesh(tmlFile, rootBlock, global_object):
	global used_materials, current_texture_index

	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_new_mesh_guid.bytes_le)
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	current_texture_index = 0
	used_materials = []

	mesh_name = get_str(global_object.name)
	material_id = 0
	channel_id = 0

	if global_object.type == 'ARMATURE':
		mesh_block_array = [get_nx3_mesh_block(tmlFile, rootBlock, object) for object in global_object.children if object.type == 'MESH']
	elif global_object.type == 'MESH':
		mesh_block_array = [get_nx3_mesh_block(tmlFile, rootBlock, global_object)]
	else:
		print("No mesh to export !")
		block.delete()
		return None

	ani_time_array = []
	ani_matrix_array = []
	visi_time_array = []
	visi_value_array = []
	#fx_array = []
	#mesh_children_array = []

	#string  mesh_name
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataString(0, mesh_name)
	block.addBlock(subBlock)

	#dword  material_id
	subBlock.create(fieldInfo.getField(1), rootBlock)
	subBlock.setDataInt(0, material_id)
	block.addBlock(subBlock)

	#dword  channel_id
	subBlock.create(fieldInfo.getField(2), rootBlock)
	subBlock.setDataInt(0, channel_id)
	block.addBlock(subBlock)

	#nx3_mesh_block  mesh_block_array
	subBlock.create(fieldInfo.getField(3), rootBlock)
	for i in range(len(mesh_block_array)):
		subBlock.addBlock(mesh_block_array[i])
	block.addBlock(subBlock)

	#dword ani_time_array[]
	subBlock.create(fieldInfo.getField(4), rootBlock)
	ani_time_array.setElementNumber(subBlock, len())
	for i in range(len(ani_time_array)):
		subBlock.setDataInt(i, ani_time_array[i])
	block.addBlock(subBlock)

	#float ani_matrix_array[]
	subBlock.create(fieldInfo.getField(5), rootBlock)
	ani_matrix_array.setElementNumber(subBlock, len())
	for i in range(len(ani_matrix_array)):
		subBlock.setDataFloat(i, ani_matrix_array[i])
	block.addBlock(subBlock)

	#dword visi_time_array[]
	subBlock.create(fieldInfo.getField(6), rootBlock)
	visi_time_array.setElementNumber(subBlock, len())
	for i in range(len(visi_time_array)):
		subBlock.setDataInt(i, visi_time_array[i])
	block.addBlock(subBlock)

	#float visi_value_array[]
	subBlock.create(fieldInfo.getField(7), rootBlock)
	visi_value_array.setElementNumber(subBlock, len())
	for i in range(len(visi_value_array)):
		subBlock.setDataFloat(i, visi_value_array[i])
	block.addBlock(subBlock)

	#nx3_fx fx_array[]
	subBlock.create(fieldInfo.getField(8), rootBlock)
	#unsupported, nothing to add
	block.addBlock(subBlock)

	#nx3_new_mesh mesh_children_array[]
	subBlock.create(fieldInfo.getField(9), rootBlock)
	#unsupported, nothing to add
	block.addBlock(subBlock)

	return block


def get_nx3_mesh_tm(tmlFile, rootBlock, bone):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mesh_tm_guid.bytes_le)
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	name = get_str(bone.name)

	bone_global_matrix = (bone.id_data.matrix_world.inverted() * bone.matrix).inverted().transposed()
	tm = [val for vect in bone_global_matrix for val in vect]

	#string  name
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataString(0, name)
	block.addBlock(subBlock)

	#float  tm[16]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	for i in range(16):
		subBlock.setDataFloat(i, tm[i])
	block.addBlock(subBlock)

	return block


def write_nx3_new_mesh_header(tmlFile, rootBlock):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_new_mesh_header_guid.bytes_le)
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	objects = [object for object in bpy.data.objects if object.type == 'ARMATURE']

	if len(objects) == 0:
		objects = [object for object in bpy.data.objects if object.type == 'MESH']

	mesh_array = [get_nx3_new_mesh(tmlFile, rootBlock, object) for object in objects]
	mesh_tm_array = [get_nx3_mesh_tm(tmlFile, rootBlock, bone) for armature in bpy.data.objects if armature.type == 'ARMATURE' for bone in armature.pose.bones]

	#nx3_new_mesh mesh_array[]
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	for mesh in mesh_array:
		subBlock.addBlock(mesh)
	block.addBlock(subBlock)

	#float  tm[16]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	for mesh_tm in mesh_tm_array:
		subBlock.addBlock(mesh_tm)
	block.addBlock(subBlock)

	write_mtl_header(tmlFile, rootBlock)

	rootBlock.addBlock(block)


def write(filename):
	(tmlFile, rootBlock) = load_btrfdom()

	write_version(tmlFile, rootBlock)
	write_nx3_new_mesh_header(tmlFile, rootBlock)

	writer = BtrfParser()
	writer.create(tmlFile)
	writer.writeFile(filename, rootBlock)
	#rootBlock.dumpToStdout()
