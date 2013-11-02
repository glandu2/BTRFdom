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
import uuid
import mathutils
from .btrfdom import BtrfParser, TmlFile
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


def info(str):
	print(('btrfdom: Info: ' + str))


def warn(str):
	print(('btrfdom: Warning: ' + str))


def error(str):
	print(('btrfdom: Error: ' + str))
	raise(str)


def create_anim_fcurve(action, data_path, index, count):
	keyframe = action.fcurves.new(data_path, index)
	keyframe.keyframe_points.add(count)
	for point in keyframe.keyframe_points:
		point.interpolation = 'LINEAR'
	return keyframe.keyframe_points


def check_version(rootBlock):
	block = rootBlock.getBlockByGuid(nx3_version_header_guid.bytes_le)

	if block:
		version = block.getBlock(0).getDataInt(0)
	else:
		info('there was no version information in this file')
		version = 0

	if version != 65536:
		warn('The version is not 65536, the model may not be read correctly')


def read_materials(rootBlock, file_dir):
	nx3_mtl_header_block = rootBlock.getBlockByGuid(nx3_mtl_header_guid.bytes_le)
	mtl_template_array = nx3_mtl_header_block.getBlock(0)
	material_array = [mtl_template_array.getBlock(i) for i in range(mtl_template_array.getElementNumber())]

	#contain rappelz mtl_id <=> blender material id convertion
	materials = {}

	for material_block in material_array:
		sub_mtl_block_template_array = material_block.getBlock(0)
		material_data_array = [sub_mtl_block_template_array.getBlock(i) for i in range(sub_mtl_block_template_array.getElementNumber())]
		for material_data_block in material_data_array:
			mtl_name = material_data_block.getBlock(0).getDataString(0)
			texture_name = material_data_block.getBlock(1).getDataString(0)
			mtl_id = material_data_block.getBlock(2).getDataInt(0)
			# channel_id = material_data_block.getBlock(3).getDataInt(0)
			# power = getDataFloat(getBlock(material_data_block, 4), 0)
			self_illumi = material_data_block.getBlock(5).getDataFloat(0)
			# smoothing = getDataChar(getBlock(material_data_block, 6), 0)
			# ambient = getDataInt(getBlock(material_data_block, 7), 0)
			# diffuse = getDataInt(getBlock(material_data_block, 8), 0)
			# specular = getDataInt(getBlock(material_data_block, 9), 0)

			texture_name = os.path.basename(texture_name.replace('\\', '/'))

			texture_file = file_dir + '/' + texture_name
			if not os.path.exists(texture_file):
				texture_file = file_dir + '/' + os.path.splitext(texture_name)[0] + ".dds"
			if not os.path.exists(texture_file):
				texture_file = file_dir + '/' + os.path.splitext(texture_name)[0] + ".tga"
			if not os.path.exists(texture_file):
				texture_file = file_dir + '/' + os.path.splitext(texture_name)[0] + ".png"

			texture_name = os.path.basename(texture_file)

			# info("Reading material %s with texture %s" % (mtl_name, texture_file))

			material = bpy.data.materials.new(mtl_name)

			try:
				image = bpy.data.images.load(texture_file)
			except:
				image = None
				warn("Could not load texture file %s" % texture_file)

			if mtl_id not in materials:
				materials[mtl_id] = []

			materials[mtl_id].append((material, image))

			material.emit = self_illumi
			material.use_face_texture = True
			material.use_face_texture_alpha = True

			if image:
				texture_slot = material.texture_slots.add()
				texture = bpy.data.textures.new(texture_name, 'IMAGE')
				texture.image = image
				texture.use_alpha = True

				texture_slot.texture = texture
				texture_slot.texture_coords = 'UV'
				texture_slot.use_map_color_diffuse = True
				texture_slot.use_map_color_emission = True

	return materials


def read_bones_weight(bone_block_template_array, vertices):
	bones_weight = {}
	for i in range(bone_block_template_array.getElementNumber()):
		bone_block = bone_block_template_array.getBlock(i)
		name = bone_block.getBlock(0).getDataString(0)
		weight_array = [bone_block.getBlock(1).getDataFloat(i) for i in range(bone_block.getBlock(1).getElementNumber())]
		offset_array = [bone_block.getBlock(2).getDataFloat(i) for i in range(bone_block.getBlock(2).getElementNumber())]

		# info("Mesh %s: reading vertex weight for bone %s" % (object.name, name))

		if int(len(offset_array) / 3) != int(len(weight_array) / 2):
			warn("Bone %s in object %s has a wrong number of offsets or weights, ignoring offsets" % (name, object.name))
			offset_array = []

		bones_weight[name] = [[vertices[int(vertex_index)].index, weight] for vertex_index, weight in zip(*[iter(weight_array)] * 2)]

	return bones_weight


def read_mesh_block(mesh_block_template, mesh_object, bm, mtl_textures):
	texture_index = mesh_block_template.getBlock(0).getDataInt(0)
	if mesh_block_template.getBlock(1).getElementNumber() == 0:
		print("Empty mesh block, ignoring")
		return

	if mesh_block_template.getBlock(1).getElementNumber() > 1:
		print("Mesh block has multiple mesh frame, this is not supported, using first frame")

	mesh_data = mesh_block_template.getBlock(1).getBlock(0)
	#time_value = mesh_data.getBlock(0).getDataInt(0)
	vertex_array = [mesh_data.getBlock(1).getDataFloat(i) for i in range(mesh_data.getBlock(1).getElementNumber())]
	normal_array = [mesh_data.getBlock(2).getDataFloat(i) for i in range(mesh_data.getBlock(2).getElementNumber())]
	texel_array = [mesh_data.getBlock(3).getDataFloat(i) for i in range(mesh_data.getBlock(3).getElementNumber())]

	bone_block_template_array = mesh_data.getBlock(5)

	tm = [mesh_data.getBlock(6).getDataFloat(i) for i in range(16)]

	matrix = mathutils.Matrix(((tm[0], tm[4], tm[8], tm[12]),
								(tm[1], tm[5], tm[9], tm[13]),
								(tm[2], tm[6], tm[10], tm[14]),
								(tm[3], tm[7], tm[11], tm[15])))

	face_array_template = mesh_block_template.getBlock(2)
	face_array = face_array_template.getDataShortPtr()

	vertex_data = [(vertex_array[int(i * 3)], vertex_array[int(i * 3 + 1)], vertex_array[int(i * 3 + 2)]) for i in range(int(mesh_data.getBlock(1).getElementNumber() / 3))]
	normal_data = [(-normal_array[int(i * 3)], -normal_array[int(i * 3 + 1)], -normal_array[int(i * 3 + 2)]) for i in range(int(mesh_data.getBlock(2).getElementNumber() / 3))]
	texel_data = [(texel_array[int(i * 2)], 1 - texel_array[int(i * 2 + 1)]) for i in range(int(mesh_data.getBlock(3).getElementNumber() / 2))]
	face_array = [(face_array[int(i * 3 + 2)], face_array[int(i * 3 + 1)], face_array[int(i * 3)]) for i in range(int(mesh_block_template.getBlock(2).getElementNumber() / 3))]

	if len(texel_data) > 0 and mtl_textures is not None:
		has_texture = True
	else:
		has_texture = False

	if len(normal_data) > 0:
		has_normals = True
	else:
		has_normals = False

	if has_texture:
		try:
			material = mtl_textures[texture_index][0]
			material_image = mtl_textures[texture_index][1]
		except:
			warn("Material %d not found for object %s" % (texture_index, mesh_object.name))
			material = None
			material_image = None
			has_texture = False

	if has_texture:
		try:
			material_names = [m.name for m in mesh_object.data.materials]
			material_id = material_names.index(material.name)
		except ValueError:
			mesh_object.data.materials.append(bpy.data.materials[material.name])
			material_id = len(mesh_object.data.materials) - 1

	vertices = []
	for vertex in vertex_data:
		vert = bm.verts.new(vertex)
		if has_normals:
			vert.normal = normal_data[vert.index]
		vertices.append(vert)

	bm.verts.index_update()

	if has_texture:
		uv_layer = bm.loops.layers.uv.verify()
		tex_layer = bm.faces.layers.tex.verify()

	for face_indices in face_array:
		try:
			face = bm.faces.new([vertices[i] for i in face_indices])
		except:
			continue
		if has_texture:
			face.material_index = material_id
			face[tex_layer].image = material_image
			face.loops[0][uv_layer].uv = texel_data[face_indices[1]]
			face.loops[1][uv_layer].uv = texel_data[face_indices[2]]
			face.loops[2][uv_layer].uv = texel_data[face_indices[0]]

	#2.64: 0,0  1,1  2,2
	#2.68: 0,1  1,2  2,0

	mesh_object.matrix_world = matrix

	if has_texture:
		del uv_layer
		del tex_layer

	# object.data.tessface_uv_textures.new()
	# object.data.from_pydata(vertex_data, [], face_array)
	# object.data.vertices.foreach_set("normal", normal_data)
	# for uv1, uv2, uv3 in zip(*[iter(vars)]*2):

	return read_bones_weight(bone_block_template_array, vertices)


def time_to_frame(time):
	return int(time / 5000 * bpy.context.scene.render.fps * bpy.context.scene.render.fps_base)


def read_mesh(mesh_template, materials, armature):
	name = mesh_template.getBlock(0).getDataString(0)
	material_id = mesh_template.getBlock(1).getDataInt(0)
	# channel_id = mesh_template.getBlock(2).getDataInt(0)

	try:
		mtl_textures = materials[material_id]
	except:
		mtl_textures = None
		if material_id != -1:
			warn("Unable to find material for mesh %s: mtl_id %d" % (name, material_id))

	mesh_block_array = mesh_template.getBlock(3)

	ani_time_array = [mesh_template.getBlock(4).getDataInt(i) for i in range(mesh_template.getBlock(4).getElementNumber())]
	ani_matrix_array = [mesh_template.getBlock(5).getDataFloat(i) for i in range(mesh_template.getBlock(5).getElementNumber())]

	visi_time_array = [mesh_template.getBlock(6).getDataInt(i) for i in range(mesh_template.getBlock(6).getElementNumber())]
	visi_value_array = [mesh_template.getBlock(7).getDataFloat(i) for i in range(mesh_template.getBlock(7).getElementNumber())]

	mesh_children_array = [mesh_template.getBlock(9).getBlock(i) for i in range(mesh_template.getBlock(9).getElementNumber())]

	mesh = bpy.data.meshes.new(name)
	mesh_object = bpy.data.objects.new(name, mesh)
	bpy.context.scene.objects.link(mesh_object)
	mesh_object.parent = armature
	armature_modifier = mesh_object.modifiers.new(armature.name, 'ARMATURE')
	armature_modifier.object = armature

	# mesh data handling

	bm = bmesh.new()
	bones_weight = []

	mesh_blocks = [mesh_block_array.getBlock(i) for i in range(mesh_block_array.getElementNumber())]
	for i, mesh_block in enumerate(mesh_blocks):
		bones_weight.append(read_mesh_block(mesh_block, mesh_object, bm, mtl_textures))

	bm.to_mesh(mesh)
	bm.free()
	del bm

	# bones weights handling

	bpy.ops.object.mode_set(mode='EDIT')

	for bone_weights in bones_weight:
		for bone_name, weight_infos in list(bone_weights.items()):
			if bone_name not in list(armature.data.edit_bones.keys()):
				bone = armature.data.edit_bones.new(bone_name)
				bone.head = (0, 0, 0)
				bone.tail = (0, 1, 0)

			if bone_name not in list(mesh_object.vertex_groups.keys()):
				vertex_group = mesh_object.vertex_groups.new(bone_name)
			else:
				vertex_group = mesh_object.vertex_groups[bone_name]

			for vertex_index, weight in weight_infos:
				vertex_group.add([vertex_index], weight, 'ADD')

	armature.data.update_tag()

	bpy.ops.object.mode_set(mode='OBJECT')

	#animation handling
	if len(ani_time_array) > 0:
		mesh_object.rotation_mode = 'QUATERNION'
		anim_data = mesh_object.animation_data_create()
		anim_data.action = bpy.data.actions.new(name + "_matrix_anim")
		anim_action = anim_data.action

		location_keyframes = [None] * 3
		location_keyframes[0] = create_anim_fcurve(anim_action, "location", 0, len(ani_time_array))
		location_keyframes[1] = create_anim_fcurve(anim_action, "location", 1, len(ani_time_array))
		location_keyframes[2] = create_anim_fcurve(anim_action, "location", 2, len(ani_time_array))

		rotation_keyframes = [None] * 4
		rotation_keyframes[0] = create_anim_fcurve(anim_action, "rotation_quaternion", 0, len(ani_time_array))
		rotation_keyframes[1] = create_anim_fcurve(anim_action, "rotation_quaternion", 1, len(ani_time_array))
		rotation_keyframes[2] = create_anim_fcurve(anim_action, "rotation_quaternion", 2, len(ani_time_array))
		rotation_keyframes[3] = create_anim_fcurve(anim_action, "rotation_quaternion", 3, len(ani_time_array))

		scale_keyframes = [None] * 3
		scale_keyframes[0] = create_anim_fcurve(anim_action, "scale", 0, len(ani_time_array))
		scale_keyframes[1] = create_anim_fcurve(anim_action, "scale", 1, len(ani_time_array))
		scale_keyframes[2] = create_anim_fcurve(anim_action, "scale", 2, len(ani_time_array))

		for i, time in enumerate(ani_time_array):
			frame_id = time_to_frame(time)
			location = mathutils.Vector((ani_matrix_array[i * 12 + 9], ani_matrix_array[i * 12 + 10], ani_matrix_array[i * 12 + 11]))

			rot_matrix = mathutils.Matrix(((ani_matrix_array[i * 12 + 0], ani_matrix_array[i * 12 + 3], ani_matrix_array[i * 12 + 6]),
											(ani_matrix_array[i * 12 + 1], ani_matrix_array[i * 12 + 4], ani_matrix_array[i * 12 + 7]),
											(ani_matrix_array[i * 12 + 2], ani_matrix_array[i * 12 + 5], ani_matrix_array[i * 12 + 8])))

			location_keyframes[0][i].co = frame_id, location[0]
			location_keyframes[1][i].co = frame_id, location[1]
			location_keyframes[2][i].co = frame_id, location[2]

			rotation_keyframes[0][i].co = frame_id, rot_matrix.to_quaternion()[0]
			rotation_keyframes[1][i].co = frame_id, rot_matrix.to_quaternion()[1]
			rotation_keyframes[2][i].co = frame_id, rot_matrix.to_quaternion()[2]
			rotation_keyframes[3][i].co = frame_id, rot_matrix.to_quaternion()[3]

			scale_keyframes[0][i].co = frame_id, rot_matrix.to_scale()[0]
			scale_keyframes[1][i].co = frame_id, rot_matrix.to_scale()[1]
			scale_keyframes[2][i].co = frame_id, rot_matrix.to_scale()[2]

		bpy.context.scene.frame_end = time_to_frame(ani_time_array[len(ani_time_array) - 1])

	# visibility handling (object transparency)
	if len(visi_time_array) > 0:
		visibility_keyframes = [None] * len(mesh_object.data.materials)
		for i, material in enumerate(mesh_object.data.materials):
			material.use_transparency = True
			material.transparency_method = 'Z_TRANSPARENCY'
			anim_data = material.animation_data_create()
			anim_data.action = bpy.data.actions.new(name + "_visibility_anim")
			anim_action = anim_data.action

			visibility_keyframes[i] = create_anim_fcurve(anim_action, "alpha", 0, len(visi_time_array))

		for i, time in enumerate(visi_time_array):
				frame_id = time_to_frame(time)
				for visibility_keyframes_per_mtl in visibility_keyframes:
					visibility_keyframes_per_mtl[i].co = frame_id, visi_value_array[i]

		bpy.context.scene.frame_end = max(bpy.context.scene.frame_end, time_to_frame(visi_time_array[len(visi_time_array) - 1]))

	# children handling (recursive)
	for children in mesh_children_array:
		read_mesh(children, materials, armature).parent = mesh_object

	return mesh_object


def read_bones_tm_matrix(bone_tm, armature):
	name = bone_tm.getBlock(0).getDataString(0)

	tm = [bone_tm.getBlock(1).getDataFloat(i) for i in range(16)]

	#lint:disable
	matrix = mathutils.Matrix(((tm[0], tm[4], tm[8], tm[12]),
								(tm[1], tm[5], tm[9], tm[13]),
								(tm[2], tm[6], tm[10], tm[14]),
								(tm[3], tm[7], tm[11], tm[15])))
	#lint:enable

	# try:
	# transposition problem with rotation matrix
	rotation = matrix.inverted().to_3x3()
	translation = matrix.inverted().to_translation()

	#Don't use directly the 4x4 matrix as it lead to bad results
	armature.data.edit_bones[name].transform(rotation)
	armature.data.edit_bones[name].translate(translation)
	# except:
		# print("Bone %s not found in armature %s" % (name, armature.name))


def read_mesh_header(rootBlock, materials, filename):
	nx3_mesh_header_block = rootBlock.getBlockByGuid(nx3_new_mesh_header_guid.bytes_le)
	mesh_template_array = nx3_mesh_header_block.getBlock(0)
	bones_tm_template_array = nx3_mesh_header_block.getBlock(1)

	mesh_array = [mesh_template_array.getBlock(i) for i in range(mesh_template_array.getElementNumber())]

	if mesh_template_array.getElementNumber() == 0:
		print("No mesh in the file !")
		return

	# Create armature with the filename as name, then an object with the same name
	armature_data = bpy.data.armatures.new(filename)
	armature = bpy.data.objects.new(filename, armature_data)
	bpy.context.scene.objects.link(armature)
	bpy.context.scene.update()
	bpy.context.scene.objects.active = armature
	armature.select = True

	for mesh in mesh_array:
		read_mesh(mesh, materials, armature)

	bones_tm_array = [bones_tm_template_array.getBlock(i) for i in range(bones_tm_template_array.getElementNumber())]

	bpy.ops.object.mode_set(mode='EDIT')
	for bone_tm in bones_tm_array:
		read_bones_tm_matrix(bone_tm, armature)
	bpy.ops.object.mode_set(mode='OBJECT')


def read(nx3_filename):
	info("Reading file %s" % nx3_filename)

	script_dir = os.path.dirname(os.path.abspath(__file__))

	tmlFile = TmlFile()
	tmlFile.create()
	tmlFile.parseFile(script_dir + "/nx3.tml")
	tmlFile.parseFile(script_dir + "/nobj.tml")

	parser = BtrfParser()
	parser.create(tmlFile)
	rootBlock = parser.readFile(nx3_filename)

	if rootBlock is None:
		error("Could not read nx3 file")
		return

	#check_version(rootBlock)
	materials = read_materials(rootBlock, os.path.dirname(nx3_filename))
	read_mesh_header(rootBlock, materials, os.path.basename(nx3_filename))
