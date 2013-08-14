/*
 * BTRFdom - Rappelz BTRF Document Object Model
 * By Glandu2
 * Copyright 2013 Glandu2
 *
 * This file is part of BTRFdom.
 * BTRFdom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BTRFdom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with BTRFdom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "File.h"
#include <string.h>

File::File() {
	file = 0;
	memoryData = 0;
	readPtr = 0;
	fileSize = 0;
}

File::~File() {
	close();
}

bool File::open(const char* filename) {
	file = fopen(filename, "rb");
	if(!file)
		return false;

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	memoryData = new char[fileSize];
	size_t nbRead = 0, totalRead = 0;
	while(totalRead < fileSize) {
		nbRead = fread((char*)memoryData + totalRead, 1, fileSize - totalRead, file);
		if(nbRead < 0 || (nbRead == 0 && !feof(file))) {
			perror("Can't read input file");
			close();
			return false;
		} else if(feof(file))
			break;
		totalRead += nbRead;
	}
	fclose(file);
	file = 0;

	readPtr = 0;

	return true;
}

void File::close() {
	delete[] (char*)memoryData;
	memoryData = 0;
}
