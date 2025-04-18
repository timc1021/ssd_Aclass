#include "SSDController.h"
#include <sstream>
#include <iomanip>
#include <exception>

SSDController::SSDController(std::shared_ptr<FileTextIOInterface> data)
	: SSDControllerInterface(std::move(data)) {}

void SSDController::writeLBA(int lba, uint32_t value) {
	if (lba < 0 || lba >= LBA_SIZE)
		throw std::invalid_argument("LBA must be between 0 and 99");

	if (ssdData.empty())
		getSsdDataFromFile();

	ssdData[lba] = value;
	saveSsdDataToFile();
}

uint32_t SSDController::readLBA(int lba) {
	if (lba < 0 || lba >= LBA_SIZE)
		throw std::invalid_argument("LBA must be between 0 and 99");

	if (ssdData.empty())
		getSsdDataFromFile();

	return ssdData[lba];
}

void SSDController::getSsdDataFromFile() {
	std::string rawData = data->loadFromFile();
	std::istringstream iss(rawData);
	std::string line;

	if (rawData.empty()) {
		ssdData.resize(LBA_SIZE, 0x00000000);
		return;
	}

	while (std::getline(iss, line)) {
		ssdData.push_back(std::stoul(line, nullptr, 16));
	}
}

void SSDController::saveSsdDataToFile() {
	std::ostringstream oss;
	for (const auto& d : ssdData) {
		oss << "0x" << std::hex << std::setw(8)
			<< std::setfill('0') << std::uppercase << d << '\n';
	}
	data->saveToFile(oss.str());
}