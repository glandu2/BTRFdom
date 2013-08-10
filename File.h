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
