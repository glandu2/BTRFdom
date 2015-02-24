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
import binascii
import struct

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


# Represent a vertex with it's normal and UV coords. Vertex, normals and UV coords cannot be separated in Rappelz NX3 format.
class VertexInfo:
	def __init__(self, vertex_index, vertex, normal, texel=[0, 0]):
		self.vertex_index = vertex_index
		self.vertex = vertex
		self.normal = normal
		self.texel = texel

	def __hash__(self):
		crc = 0
		for co in self.vertex:
			crc = binascii.crc32(struct.pack("l", int(co * 1000)), crc)
		for co in self.normal:
			crc = binascii.crc32(struct.pack("l", int(co * 1000)), crc)
		for co in self.texel:
			crc = binascii.crc32(struct.pack("l", int(co * 1000)), crc)

		return crc

	def __eq__(self, other):
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


class BoneInfo:
	__slots__ = ("name", "global_matrix")

	def __init__(self, name, global_matrix):
		self.name = name
		self.global_matrix = global_matrix


def info(str):
	print(('btrfdom: Info: ' + str))


def warn(str):
	print(('btrfdom: Warning: ' + str))


def error(str):
	print(('btrfdom: Error: ' + str))
	raise(str)


def get_parent_mesh(mesh_object):
	while mesh_object.parent is not None:
		mesh_object = mesh_object.parent
		if mesh_object.type == 'MESH':
			return mesh_object


def get_children_meshes(blender_object):
	children = []
	for child in blender_object.children:
		if child.type == 'MESH':
			children.append(child)
		else:
			children.extend(get_children_meshes(child))

	return children


def get_materials_bones_from_object(blender_object, materials_info, bones_info):
	if blender_object.type == 'MESH':
		for material_index, material in enumerate(blender_object.material_slots):
			if blender_object.name not in materials_info:
				materials_info[blender_object.name] = []
			materials_info[blender_object.name].append(Material(material_index, list(materials_info.keys()).__len__() - 1, 0, material.material))

	bone_name_unique_check = set()
	if blender_object.type == 'ARMATURE':
		for bone in blender_object.pose.bones:
			bone_global_matrix = (bone.id_data.matrix_world.inverted() * bone.matrix).inverted().transposed()
			tm = [val for vect in bone_global_matrix for val in vect]
			if bone.name not in bone_name_unique_check:
				bone_name_unique_check.add(bone.name)
				bones_info.append(BoneInfo(bone.name, tm))
			else:
				raise "Bones names must be unique trough the whole file. Several bones have the name %s" % bone.name


def get_materials_bones():
	materials_info = {}
	bones_info = []

	for obj in bpy.data.objects:
		get_materials_bones_from_object(obj, materials_info, bones_info)

	return [materials_info, bones_info]


def get_texture_filename(material):
	#try:
	if material.texture_slots:
		texture_slot = material.texture_slots[0]
		if texture_slot is not None:
			texture = texture_slot.texture
			if texture is not None and texture.type == 'IMAGE':
				return os.path.basename(texture.image.filepath)
	#except:
		#pass
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

	mtl_name = material_info.material.name
	texture_name = get_texture_filename(material_info.material)
	mtl_id = material_info.material_id
	channel_id = material_info.channel_id
	power = 0
	self_illumi = material_info.material.emit
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


def get_mtl_data(tmlFile, rootBlock, materials_info):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mtl_guid.bytes_le)

	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	arrayBlock = BtrfBlock()
	arrayBlock.create(fieldInfo.getField(0), rootBlock)
	#iterate over all used materials

	for materials_info_per_object in list(materials_info.values()):
		for material_info in materials_info_per_object:
			subBlock = get_mtl_block(tmlFile, rootBlock, material_info)
			arrayBlock.addBlock(subBlock)

	block.addBlock(arrayBlock)
	return block


def write_mtl_header(tmlFile, rootBlock, materials_info):
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mtl_header_guid.bytes_le)

	#Create a block that will contain the data of the template
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	arrayBlock = BtrfBlock()
	arrayBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock = get_mtl_data(tmlFile, rootBlock, materials_info)
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

	bone_name = vertex_group.name
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
	ordered_vertex_info = sorted(vertex_info_array, key=vertex_info_array.__getitem__)
	vertex_array = [coord for vertex_info in ordered_vertex_info for coord in vertex_info.vertex]
	normal_array = [coord for vertex_info in ordered_vertex_info for coord in vertex_info.normal]

	if has_texel is True:
		texel_array = [(vertex_info.texel[0], 1 - vertex_info.texel[1]) for vertex_info in ordered_vertex_info]
	else:
		texel_array = []

	color_array = []

	vertex_indices = [vertex_info.vertex_index for vertex_info in ordered_vertex_info]
	bone_block = [get_nx3_weight_frame(tmlFile, rootBlock, vertex_group, vertex_indices) for vertex_group in vertex_groups]
	mesh_tm = [val for vect in mesh_matrix.transposed() for val in vect]

	#dword  time_value
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataInt(0, time_value)
	block.addBlock(subBlock)

	#float  vertex_array[]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	subBlock.setElementNumber(len(vertex_array))
	for i in range(len(vertex_array)):
		subBlock.setDataFloat(i, vertex_array[i])
	block.addBlock(subBlock)

	#float  normal_array[]
	subBlock.create(fieldInfo.getField(2), rootBlock)
	subBlock.setElementNumber(len(normal_array))
	for i in range(len(normal_array)):
		subBlock.setDataFloat(i, normal_array[i])
	block.addBlock(subBlock)

	#float  texel_array[]
	subBlock.create(fieldInfo.getField(3), rootBlock)
	subBlock.setElementNumber(int(len(texel_array) * 2))
	for i in range(len(texel_array)):
		subBlock.setDataFloat(int(i * 2), texel_array[i][0])
		subBlock.setDataFloat(int(i * 2 + 1), texel_array[i][1])
	block.addBlock(subBlock)

	#dword  color_array[]
	subBlock.create(fieldInfo.getField(4), rootBlock)
	subBlock.setElementNumber(len(color_array))
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


def get_nx3_mesh_block(tmlFile, rootBlock, mesh_object, mesh_data, mesh_block_faces, tex_index):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mesh_block_guid.bytes_le)

	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	if tex_index is None:
		has_texture = False
	else:
		has_texture = True

	if has_texture:
		texture_index = tex_index
	else:
		texture_index = 0

	vertex_groups = mesh_object.vertex_groups
	vertex_info_array = {}
	index_array = []

	for face in mesh_block_faces:
		for i, vertex_index in reversed(list(enumerate(face.vertices))):    # reversed instead of enumerate else normals are wrong
			if has_texture:
				uv_texture = mesh_data.tessface_uv_textures[0]
				vertex_info = VertexInfo(vertex_index, mesh_data.vertices[vertex_index].co, mesh_data.vertices[vertex_index].normal, uv_texture.data[face.index].uv[i])
			else:
				vertex_info = VertexInfo(vertex_index, mesh_data.vertices[vertex_index].co, mesh_data.vertices[vertex_index].normal)

			#index = index_of_vertex_info(vertex_info_array, vertex_info)
			if vertex_info in vertex_info_array:
				index_array.append(vertex_info_array[vertex_info])
			else:
				index = len(vertex_info_array)
				vertex_info_array[vertex_info] = index
				index_array.append(index)

	mesh_frame = get_nx3_mesh_frame(tmlFile, rootBlock, mesh_object.matrix_world, vertex_info_array, vertex_groups, has_texture)

	#dword  texture_index
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataInt(0, texture_index)
	block.addBlock(subBlock)

	#nx3_mesh_frame  mesh_frame_array[]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	subBlock.addBlock(mesh_frame)
	block.addBlock(subBlock)

	#word  index_buffer_array[]
	subBlock.create(fieldInfo.getField(2), rootBlock)
	subBlock.setElementNumber(len(index_array))
	for i in range(len(index_array)):
		subBlock.setDataShort(i, index_array[i])
	block.addBlock(subBlock)

	return block


def get_nx3_new_mesh(tmlFile, rootBlock, mesh_object, materials_info):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_new_mesh_guid.bytes_le)
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	mesh_data = mesh_object.to_mesh(bpy.context.scene, True, 'PREVIEW')
	mesh_data.update(calc_tessface=True)

	if mesh_object.name in materials_info and len(materials_info[mesh_object.name]) > 0 and len(mesh_data.tessface_uv_textures) > 0:
		has_material = True
	else:
		has_material = False

	mesh_name = mesh_object.name

	if has_material is True:
		material_id = materials_info[mesh_object.name][0].material_id
		channel_id = 0
	else:
		material_id = -1
		channel_id = 0

	if has_material is True:
		mesh_blocks_faces = [[] for _ in range(len(materials_info[mesh_object.name]))]
		# Separate faces to several mesh_block to have at most one texture per mesh_block
		for face in mesh_data.tessfaces:
			mesh_blocks_faces[face.material_index].append(face)

		mesh_block_array = [get_nx3_mesh_block(tmlFile, rootBlock, mesh_object, mesh_data, mesh_block_faces, texture_index) for texture_index, mesh_block_faces in enumerate(mesh_blocks_faces)]
	else:
		mesh_block_array = [get_nx3_mesh_block(tmlFile, rootBlock, mesh_object, mesh_data, mesh_data.tessfaces, None)]

	ani_time_array = []
	ani_matrix_array = []
	visi_time_array = []
	visi_value_array = []
	#fx_array = []
	children_objects = get_children_meshes(mesh_object)
	mesh_children_array = [get_nx3_new_mesh(tmlFile, rootBlock, child_object, materials_info) for child_object in children_objects]

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
	subBlock.setElementNumber(len(ani_time_array))
	for i in range(len(ani_time_array)):
		subBlock.setDataInt(i, ani_time_array[i])
	block.addBlock(subBlock)

	#float ani_matrix_array[]
	subBlock.create(fieldInfo.getField(5), rootBlock)
	subBlock.setElementNumber(len(ani_matrix_array))
	for i in range(len(ani_matrix_array)):
		subBlock.setDataFloat(i, ani_matrix_array[i])
	block.addBlock(subBlock)

	#dword visi_time_array[]
	subBlock.create(fieldInfo.getField(6), rootBlock)
	subBlock.setElementNumber(len(visi_time_array))
	for i in range(len(visi_time_array)):
		subBlock.setDataInt(i, visi_time_array[i])
	block.addBlock(subBlock)

	#float visi_value_array[]
	subBlock.create(fieldInfo.getField(7), rootBlock)
	subBlock.setElementNumber(len(visi_value_array))
	for i in range(len(visi_value_array)):
		subBlock.setDataFloat(i, visi_value_array[i])
	block.addBlock(subBlock)

	#nx3_fx fx_array[]
	subBlock.create(fieldInfo.getField(8), rootBlock)
	#unsupported, nothing to add
	block.addBlock(subBlock)

	#nx3_new_mesh mesh_children_array[]
	subBlock.create(fieldInfo.getField(9), rootBlock)
	for i in range(len(mesh_children_array)):
		subBlock.addBlock(mesh_children_array[i])
	block.addBlock(subBlock)

	bpy.data.meshes.remove(mesh_data)

	return block


def get_nx3_bone_tm(tmlFile, rootBlock, bone_info):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_mesh_tm_guid.bytes_le)
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	#string  name
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	subBlock.setDataString(0, bone_info.name)
	block.addBlock(subBlock)

	#float  tm[16]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	for i in range(16):
		subBlock.setDataFloat(i, bone_info.global_matrix[i])
	block.addBlock(subBlock)

	return block


def write_nx3_new_mesh_header(tmlFile, rootBlock, bones_info, materials_info):
	#Create a block that will contain the data of the template
	fieldInfo = tmlFile.getTemplateByGuid(nx3_new_mesh_header_guid.bytes_le)
	block = BtrfBlock()
	block.create(fieldInfo, rootBlock)

	objects = [obj for obj in bpy.data.objects if obj.type == 'MESH' and get_parent_mesh(obj) is None]

	mesh_array = [get_nx3_new_mesh(tmlFile, rootBlock, mesh_object, materials_info) for mesh_object in objects]
	bone_tm_array = [get_nx3_bone_tm(tmlFile, rootBlock, bone_info) for bone_info in bones_info]

	#nx3_new_mesh mesh_array[]
	subBlock = BtrfBlock()
	subBlock.create(fieldInfo.getField(0), rootBlock)
	for mesh in mesh_array:
		subBlock.addBlock(mesh)
	block.addBlock(subBlock)

	#float  tm[16]
	subBlock.create(fieldInfo.getField(1), rootBlock)
	for bone_tm in bone_tm_array:
		subBlock.addBlock(bone_tm)
	block.addBlock(subBlock)

	rootBlock.addBlock(block)


def write(filename):
	(tmlFile, rootBlock) = load_btrfdom()

	(materials_info, bones_info) = get_materials_bones()

	write_version(tmlFile, rootBlock)
	write_mtl_header(tmlFile, rootBlock, materials_info)
	write_nx3_new_mesh_header(tmlFile, rootBlock, bones_info, materials_info)

	info("Writing file %s" % filename)
	writer = BtrfParser()
	writer.create(tmlFile)
	writer.writeFile(filename, rootBlock)
