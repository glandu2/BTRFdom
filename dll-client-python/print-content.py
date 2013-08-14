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

# This python script load the dungeon_elkassia_wallgate01.nx3
# file and then print to stdout it's content.

from ctypes import windll

btrfdll = windll.LoadLibrary("./BTRFdom")
tmlFile = btrfdll.createTmlFile()

btrfdll.parseFileTmlFile(tmlFile, b"nx3.tml")
btrfdll.parseFileTmlFile(tmlFile, b"nobj.tml")

parser = btrfdll.createBtrfParser(tmlFile)
rootBlock = btrfdll.readFileBtrfParser(parser, b"dungeon_elkassia_wallgate01.nx3")
btrfdll.dumpToStdoutBtrfRootBlock(rootBlock)
