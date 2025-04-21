#pragma once

#include "FileTextIOInterface.h"
#include <string>
#include <fstream>
#include <sstream>

class FileTextIO : public FileTextIOInterface {
public:
	explicit FileTextIO(const std::string& fileName)
		: FileTextIOInterface(fileName) {}

	std::string loadFromFile() override {
		std::ifstream inFile(fileName);
		if (!inFile.is_open()) return "";

		std::ostringstream ss;
		ss << inFile.rdbuf();
		return ss.str();
	}

	void saveToFile(std::string data) const override {
		std::ofstream outFile(fileName);
		if (!outFile.is_open()) return;

		outFile << data;
	}
};
