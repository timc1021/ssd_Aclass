#include "FileTextIO.h"
#include <fstream>
#include <sstream>

FileTextIO::FileTextIO(std::string fileName) : FileTextIOInterface(fileName) {}

std::string FileTextIO::loadFromFile() {
	std::ifstream inFile(fileName);
	if (!inFile.is_open()) {
		return "";
	}

	std::ostringstream ss;
	ss << inFile.rdbuf();
	return ss.str();
}

void FileTextIO::saveToFile(const std::string data) const {
	std::ofstream outFile(fileName);
	if (!outFile.is_open()) {
		return;
	}

	outFile << data;
}
