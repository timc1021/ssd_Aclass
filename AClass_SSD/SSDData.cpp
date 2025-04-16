#include "SSDData.h"

#include <sstream>

SSDData::SSDData(DataInterface* data) : data(data) {}

void SSDData::writeLBA(int lba, uint32_t value) { return; }

unsigned int SSDData::readLBA(int lba) {
	if (ssdData.empty()) getSsdData();

	return ssdData[lba];
}

void SSDData::getSsdData() {
	std::string rawData = data->loadFromFile();
	std::istringstream iss(rawData);
	std::string line;
	unsigned int value;

	while (std::getline(iss, line)) {
		value = std::stoul(line, nullptr, 16);
		ssdData.push_back(value);
	}
}
