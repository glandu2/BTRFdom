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

#ifndef USR_FILE_H
#define USR_FILE_H

#include <stddef.h>
#include <stdio.h>

class File
{
	public:
		File();
		bool open(const char* filename);

		template<class T>
		const T* read(size_t length);

		void close();
		const void* getPtr(size_t pos) { return (char*)memoryData + pos; }
		void setPos(size_t pos) { readPtr = pos; }
		size_t getPos() { return readPtr; }

		virtual ~File();
	protected:
	private:
		FILE* file;
		size_t fileSize;
		void* memoryData;
		size_t readPtr;
};

template<class T>
const T* File::read(size_t length) {
	if(readPtr + length > fileSize){
		length = fileSize - readPtr;
		fprintf(stderr, "WARNING: reading out of file, at pos %d, tried to read %d bytes while file only %d long !\n", readPtr, length, fileSize);
		return 0;
	}
	readPtr += length;

	return reinterpret_cast<const T*>(((char*)memoryData) + (readPtr - length));
}

#endif // USR_FILE_H
