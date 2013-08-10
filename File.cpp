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
