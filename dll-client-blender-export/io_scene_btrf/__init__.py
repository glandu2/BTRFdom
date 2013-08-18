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

bl_info = {
	"name": "Rappelz NX3 format",
	"author": "Glandu2",
	"blender": (2, 6, 4),
	"version": (0,2,0),
	"location": "File > Import-Export",
	"description": "Export to a Rappelz NX3 file",
	"category": "Import-Export"}

import bpy
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty


class ExportBTRF(bpy.types.Operator, ExportHelper):
	bl_idname = "export_mesh.nx3"
	bl_label = "Export NX3"
	bl_options = {'PRESET'}
	
	filepath = StringProperty(
			subtype='FILE_PATH',
			)
	
	filename_ext	= ".nx3";
	
	def execute(self, context):
		from . import export_btrf
		export_btrf.write(self.filepath.encode('cp1252'))
		return {'FINISHED'};

def menu_func_export(self, context):
	self.layout.operator(ExportBTRF.bl_idname, text="Rappelz NX3 (.nx3)")


def register():
	bpy.utils.register_module(__name__)

#	bpy.types.INFO_MT_file_import.append(menu_func_import)
	bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
	bpy.utils.unregister_module(__name__)

#	bpy.types.INFO_MT_file_import.remove(menu_func_import)
	bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
	register()
