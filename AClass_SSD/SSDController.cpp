#include "SSDController.h"

#include <sstream>
#include <iomanip>
#include <exception>

SSDController::SSDController(std::shared_ptr<FileTextIOInterface> data) : SSDControllerInterface(std::move(data)) {}

void SSDController::writeLBA(int lba, const uint32_t value) {
	if (lba < 0 || lba >= LBA_SIZE) throw std::invalid_argument("LBA must be greater than or equal to 0 and less than 100");
	if (ssdData.empty()) getSsdDataFromFile();

	ssdData[lba] = value;

	saveSsdDataToFile();
	return;
}

uint32_t SSDController::readLBA(const int lba) {
	if (lba < 0 || lba >= LBA_SIZE) throw std::invalid_argument("LBA must be greater than or equal to 0 and less than 100");
	if (ssdData.empty()) getSsdDataFromFile();

	return ssdData[lba];
}

void SSDController::getSsdDataFromFile() {
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
void SSDController::saveSsdDataToFile()
{
	std::stringstream ss;

	for (auto data : ssdData)
		ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << data << '\n';

	data->saveToFile(ss.str());
}
