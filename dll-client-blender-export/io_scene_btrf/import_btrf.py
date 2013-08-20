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

""" NX3 structure:
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
import bmesh
from ctypes import windll, c_float, c_int, c_short, c_byte, c_char_p, c_void_p
import ctypes
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

btrfdll = 0
tmlFile = 0
getBlock = None
getBlockByGuid = None
getElementNum = None
getString = None
getFloat = None
getInt = None
getShort = None
getChar = None
getFloatPtr = None
getIntPtr = None
getShortPtr = None
getCharPtr = None
	
def check_version(rootBlock):
	block = getBlockByGuid(rootBlock, nx3_version_header_guid.bytes_le)
	version = getInt(getBlock(block, 0), 0)
	if version != 65536:
		print('Version %d is not supported, import may fail. Version should be 65536' % version)


def read_materials(rootBlock, file_dir):
	nx3_mtl_header_block = getBlockByGuid(rootBlock, nx3_mtl_header_guid.bytes_le)
	mtl_template_array = getBlock(nx3_mtl_header_block, 0)
	material_array = [getBlock(mtl_template_array, i) for i in range(getElementNum(mtl_template_array))]
	
	#contain rappelz mtl_id <=> blender material id convertion
	mtl_ids = {}
	
	for material_block in material_array:
		sub_mtl_block_template_array = getBlock(material_block, 0)
		material_data_array = [getBlock(sub_mtl_block_template_array, i) for i in range(getElementNum(sub_mtl_block_template_array))]
		for material_data_block in material_data_array:
			mtl_name = getString(getBlock(material_data_block, 0), 0)
			texture_name = getString(getBlock(material_data_block, 1), 0)
			mtl_id = getInt(getBlock(material_data_block, 2), 0)
			# channel_id = getInt(getBlock(material_data_block, 3), 0)
			# power = getFloat(getBlock(material_data_block, 4), 0)
			# self_illumi = getFloat(getBlock(material_data_block, 5), 0)
			# smoothing = getChar(getBlock(material_data_block, 6), 0)
			# ambient = getInt(getBlock(material_data_block, 7), 0)
			# diffuse = getInt(getBlock(material_data_block, 8), 0)
			# specular = getInt(getBlock(material_data_block, 9), 0)
			
			texture_name = os.path.basename(texture_name)
			
			texture_file = file_dir + '/' + texture_name
			
			print("Reading material %s with texture %s" % (mtl_name, texture_file))
			
			material = bpy.data.materials.new(mtl_name)

			try:
				image = bpy.data.images.load(texture_file)
			except:
				image = None
				print("Could not load texture file %s" % texture_file)
				
				
			mtl_ids[mtl_id] = (material, image)
			
			texture_slot = material.texture_slots.add()
			if image:
				texture = bpy.data.textures.new(texture_name, 'IMAGE')
				texture.image = image
				texture.use_alpha = True
				
				texture_slot.texture = texture
				texture_slot.texture_coords = 'UV'
				texture_slot.use_map_color_diffuse = True
				texture_slot.use_map_color_emission = True
			else:
				texture_slot.texture = None
			
	return mtl_ids
	
def read_bones_weight(bone_block_template_array, armature, object):
	for i in range(getElementNum(bone_block_template_array)):
		bone_block = getBlock(bone_block_template_array, i)
		name = getString(getBlock(bone_block, 0), 0)
		weight_array = [ getFloat(getBlock(bone_block, 1), i) for i in range(getElementNum(getBlock(bone_block, 1))) ]
		offset_array = [ getFloat(getBlock(bone_block, 2), i) for i in range(getElementNum(getBlock(bone_block, 2))) ]
		
		print("Mesh %s: reading vertex weight for bone %s" % (object.name, name))
		
		if int(len(offset_array)/3) != int(len(weight_array)):
			print("Bone %s in object %s has a wrong number of offsets or weights, ignoring offsets" % (name, object.name))
			offset_array = []
		
		
		bpy.ops.object.mode_set(mode='EDIT')

		bone = armature.data.edit_bones.new(name)
		vertex_group = object.vertex_groups.new(name)
		for vertex_index, weight in zip(*[iter(weight_array)]*2):
			vertex_group.add([int(vertex_index)], weight, 'ADD')
			
		bone.head = (0, 0, 0)
		bone.tail = (0, 1, 0)
		armature.data.update_tag()

		bpy.ops.object.mode_set(mode='OBJECT')
		
			
def read_mesh_block(mesh_block_template, armature, name, mtl_ids):
	texture_index = getInt(getBlock(mesh_block_template, 0), 0)
	if getElementNum(getBlock(mesh_block_template, 1)) == 0:
		print("Empty mesh block, ignoring")
		return

	if getElementNum(getBlock(mesh_block_template, 1)) > 1:
		print("Mesh block has multiple mesh frame, this is not supported, using first frame")

	mesh_data = getBlock(getBlock(mesh_block_template, 1), 0)
	time_value = getInt(getBlock(mesh_data, 0), 0)
	vertex_array = [ getFloat(getBlock(mesh_data, 1), i) for i in range(getElementNum(getBlock(mesh_data, 1))) ]
	normal_array = [ getFloat(getBlock(mesh_data, 2), i) for i in range(getElementNum(getBlock(mesh_data, 2))) ]
	texel_array = [ getFloat(getBlock(mesh_data, 3), i) for i in range(getElementNum(getBlock(mesh_data, 3))) ]
	
	bone_block_template_array = getBlock(mesh_data, 5)
	
	tm = [ getFloat(getBlock(mesh_data, 6), i) for i in range(16) ]
	matrix = mathutils.Matrix(((tm[0], tm[4], tm[8],  tm[12]),
					           (tm[1], tm[5], tm[9],  tm[13]),
					           (tm[2], tm[6], tm[10], tm[14]),
					           (tm[3], tm[7], tm[11], tm[15])))
					 
	face_array_template = getBlock(mesh_block_template, 2)
	face_array = btrfdll.getDataShortPtrBtrfBlock(face_array_template)

	vertex_data = [ (vertex_array[int(i*3)], vertex_array[int(i*3+1)], vertex_array[int(i*3+2)]) for i in range(int(getElementNum(getBlock(mesh_data, 1))/3)) ]
	normal_data = [ (-normal_array[int(i*3)], -normal_array[int(i*3+1)], -normal_array[int(i*3+2)]) for i in range(int(getElementNum(getBlock(mesh_data, 2))/3)) ]
	texel_data  = [ (texel_array[int(i*2)], 1-texel_array[int(i*2+1)]) for i in range(int(getElementNum(getBlock(mesh_data, 3))/2)) ]
	face_array =  [ (face_array[int(i*3+2)], face_array[int(i*3+1)], face_array[int(i*3)]) for i in range(int(getElementNum(getBlock(mesh_block_template, 2))/3)) ]

	bm = bmesh.new()
	for vertex in vertex_data:
		vert = bm.verts.new(vertex)
		vert.normal = normal_data[vert.index]
		
	uv_layer = bm.loops.layers.uv.verify()
	bm.faces.layers.tex.verify()

	for face_indices in face_array:
		face = bm.faces.new([bm.verts[i] for i in face_indices])
		face.loops[0][uv_layer].uv = texel_data[face_indices[0]]
		face.loops[1][uv_layer].uv = texel_data[face_indices[1]]
		face.loops[2][uv_layer].uv = texel_data[face_indices[2]]

	mesh = bpy.data.meshes.new(name)
	bm.to_mesh(mesh)
	del uv_layer
	bm.free()
	del bm
	
	object = bpy.data.objects.new(name, mesh)
	bpy.context.scene.objects.link(object)
	object.parent = armature
	armature_modifier = object.modifiers.new(armature.name, 'ARMATURE')
	armature_modifier.object = armature
	
	read_bones_weight(bone_block_template_array, armature, object)
	
	# object.data.tessface_uv_textures.new()
	# object.data.from_pydata(vertex_data, [], face_array)
    # object.data.vertices.foreach_set("normal", normal_data)
	# for uv1, uv2, uv3 in zip(*[iter(vars)]*2):
					 
	object.matrix_world = matrix
	# try:
	object.data.materials.append(mtl_ids[texture_index][0])
	material_image = mtl_ids[texture_index][1]
	if material_image:
		for texture in object.data.uv_textures.active.data:
			texture.image = material_image
	# except:
		# print("Material %d not found for object %s" % (texture_index, object.name))
		
	return object

def read_mesh(mesh_template, mtl_ids):
	name = getString(getBlock(mesh_template, 0), 0)
	# material_id = getInt(getBlock(mesh_template, 1), 0)
	# channel_id = getInt(getBlock(mesh_template, 2), 0)
	
	mesh_block_array = getBlock(mesh_template, 3)

	# animation, visi, childrens are not supported
	
	armature_data = bpy.data.armatures.new(name)
	armature = bpy.data.objects.new(name, armature_data)
	bpy.context.scene.objects.link(armature)
	bpy.context.scene.update()
	bpy.context.scene.objects.active = armature
	armature.select = True
	
	mesh_blocks = [ getBlock(mesh_block_array, i) for i in range(getElementNum(mesh_block_array)) ]
	for i, mesh_block in enumerate(mesh_blocks):
		object = read_mesh_block(mesh_block, armature, name + "_" + str(i), mtl_ids)
		
	return armature

def read_bones_tm_matrix(bone_tm, armature):
	name = getString(getBlock(bone_tm, 0), 0)
	tm = [ getFloat(getBlock(bone_tm, 1), i) for i in range(16) ]
	matrix = mathutils.Matrix(((tm[0], tm[4], tm[8],  tm[12]),
							   (tm[1], tm[5], tm[9],  tm[13]),
							   (tm[2], tm[6], tm[10], tm[14]),
							   (tm[3], tm[7], tm[11], tm[15])))
	
	# try:
	# transposition problem with rotation matrix
	rotation = matrix.inverted().to_3x3()
	translation = matrix.inverted().to_translation()
	
	#Don't use directly the 4x4 matrix as it lead to bad results
	armature.data.edit_bones[name].transform(rotation)
	armature.data.edit_bones[name].translate(translation)
	# except:
		# print("Bone %s not found in armature %s" % (name, armature.name))


def read_mesh_header(rootBlock, mtl_ids):
	nx3_mesh_header_block = getBlockByGuid(rootBlock, nx3_new_mesh_header_guid.bytes_le)
	mesh_template_array = getBlock(nx3_mesh_header_block, 0)
	bones_tm_template_array = getBlock(nx3_mesh_header_block, 1)
	
	# mesh_array = [ getBlock(mesh_template_array, i) for i in range(getElementNum(mesh_template_array)) ]
	
	if getElementNum(mesh_template_array) == 0:
		print("No mesh in the file !")
		return
	elif getElementNum(mesh_template_array) > 1:
		print("Warning: more than one mesh in file, using only the first one (multiple meshes are not supported)")
	
	mesh = getBlock(mesh_template_array, 0)
	armature = read_mesh(mesh, mtl_ids)
	
	bones_tm_array = [ getBlock(bones_tm_template_array, i) for i in range(getElementNum(bones_tm_template_array)) ]
	
	bpy.ops.object.mode_set(mode='EDIT')
	for bone_tm in bones_tm_array:
		read_bones_tm_matrix(bone_tm, armature)
	bpy.ops.object.mode_set(mode='OBJECT')




def read(nx3_filename):
	if btrfdll == 0 or tmlFile == 0:
		load_btrfdom()
		
	print("Reading file %s" % nx3_filename.decode())
		
	parser = btrfdll.createBtrfParser(tmlFile)
	rootBlock = btrfdll.readFileBtrfParser(parser, nx3_filename)
	
	if rootBlock == None:
		return
		
	check_version(rootBlock)
	mtl_ids = read_materials(rootBlock, os.path.dirname(nx3_filename.decode()))
	read_mesh_header(rootBlock, mtl_ids)




def getString(block, index):
	val = btrfdll.getDataStringBtrfBlock(block, index)
	return val.decode('cp1252')
	
def getFloatPtr(block):
	ptr = btrfdll.getDataFloatPtrBtrfBlock(block)
	

def load_btrfdom():
	global btrfdll, tmlFile, getBlock, getElementNum, getString, getFloat, getInt, getShort, getChar, getBlockByGuid
	#Load the BTRFdom dll (or so on linux)
	btrfdll = windll.LoadLibrary("./BTRFdom")

	#Create a TmlFile object that will contain all known templates and their fields
	tmlFile = btrfdll.createTmlFile()

	#Read some template files
	btrfdll.parseFileTmlFile(tmlFile, b"nx3.tml")
	btrfdll.parseFileTmlFile(tmlFile, b"nobj.tml")

	#Simpler functions
	getBlock = btrfdll.getBlockBtrfBlock
	getBlock.restype = c_void_p
	getBlock.argtypes = [c_void_p, c_int]
	getElementNum = btrfdll.getElementNumberBtrfBlock
	getElementNum.restype = c_int
	getElementNum.argtypes = [c_void_p]
	#getString is implemented in python to return strings and not byte arrays
	btrfdll.getDataStringBtrfBlock.restype = c_char_p
	btrfdll.getDataStringBtrfBlock.argtypes = [c_void_p, c_int]
	getFloat = btrfdll.getDataFloatBtrfBlock
	getFloat.restype = c_float
	getFloat.argtypes = [c_void_p, c_int]
	getInt = btrfdll.getDataIntBtrfBlock
	getInt.restype = c_int
	getInt.argtypes = [c_void_p, c_int]
	getShort = btrfdll.getDataShortBtrfBlock
	getShort.restype = c_short
	getShort.argtypes = [c_void_p, c_int]
	getChar = btrfdll.getDataCharBtrfBlock
	getChar.restype = c_byte
	getChar.argtypes = [c_void_p, c_int]
	getBlockByGuid = btrfdll.getBlockByGuidBtrfRootBlock
	getBlockByGuid.restype = c_void_p
	getBlockByGuid.argtypes = [c_void_p, c_void_p]
	
	getFloatPtr = btrfdll.getDataFloatPtrBtrfBlock
	getFloatPtr.restype = ctypes.POINTER(c_float)
	getFloatPtr.argtypes = [c_void_p]
	getIntPtr = btrfdll.getDataIntPtrBtrfBlock
	getIntPtr.restype = ctypes.POINTER(c_int)
	getIntPtr.argtypes = [c_void_p]
	getShortPtr = btrfdll.getDataShortPtrBtrfBlock
	getShortPtr.restype = ctypes.POINTER(c_short)
	getShortPtr.argtypes = [c_void_p]
	getCharPtr = btrfdll.getDataCharPtrBtrfBlock
	getCharPtr.restype = ctypes.POINTER(c_byte)
	getCharPtr.argtypes = [c_void_p]
