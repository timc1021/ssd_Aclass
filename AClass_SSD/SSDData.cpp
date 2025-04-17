#include "SSDData.h"

#include <sstream>
#include <iomanip>
#include <exception>

SSDData::SSDData(DataInterface* data) : data(data) {}

void SSDData::writeLBA(int lba, uint32_t value) {
	if (lba < 0 || lba >= LBA_SIZE) throw std::invalid_argument("LBA must be greater than or equal to 0 and less than 100");
	if (ssdData.empty()) getSsdDataFromFile();

	ssdData[lba] = value;

	saveSsdDataToFile();
	return;
}

unsigned int SSDData::readLBA(int lba) {
	if (lba < 0 || lba >= LBA_SIZE) throw std::invalid_argument("LBA must be greater than or equal to 0 and less than 100");
	if (ssdData.empty()) getSsdDataFromFile();
	return ssdData[lba];
}

void SSDData::getSsdDataFromFile() {
	std::string rawData = data->loadFromFile();
	std::istringstream iss(rawData);
	std::string line;

	if (rawData == "") {
		ssdData.resize(LBA_SIZE, 0x00000000);
		return;
	}

	while (std::getline(iss, line))
		ssdData.push_back(std::stoul(line, nullptr, 16));
}
void SSDData::saveSsdDataToFile()
{
	std::stringstream ss;

	for (auto data : ssdData)
		ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << data << '\n';

	data->saveToFile(ss.str());
}
