#pragma once
#include <vector>

class Data {
	std::vector<uint32_t> nandData;
public:
	static const int LBA_SIZE = 100;

	bool loadFromFile(const std::string& filename);
	bool saveToFile(const std::string& filename) const;
};