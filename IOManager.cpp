#include "IOManager.h"

#include <fstream>

bool IOManager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer) {
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	//Seek to end
	file.seekg(0, std::ios::end);

	//Get file size
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	//Reduce filesize by any header bytes present
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	file.read((char*)buffer.data(), fileSize);
	file.close();

	return true;
}