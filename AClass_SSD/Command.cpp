#include "Command.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>

Command::Command(std::shared_ptr<SSDControllerInterface> ssdData,
	std::shared_ptr<FileTextIOInterface> outputFile)
	: ssd(std::move(ssdData)), outputFile(std::move(outputFile)) {}

void Command::execute(const std::string& cmdType, int lba, const std::string& valueHex) {
	if (cmdType == "W" || cmdType == "w") {
		executeWrite(lba, valueHex);
	}
	else if (cmdType == "R" || cmdType == "r") {
		executeRead(lba);
	}
	else if (cmdType == "E" || cmdType == "e") {
		int size = std::stoi(valueHex);
		executeErase(lba, size);
	}
	else {
		throw std::invalid_argument("Invalid command. Use 'R', 'W', or 'E'.");
	}
}


void Command::executeWrite(int lba, const std::string& valueHex) {
	if (valueHex.length() != 10 || valueHex.substr(0, 2) != "0x")
		throw std::invalid_argument("Invalid hex format. Must be '0x' followed by 8 hex digits.");

	uint32_t value = std::stoul(valueHex, nullptr, 16);
	ssd->writeLBA(lba, value);
}

void Command::executeRead(int lba) {
	uint32_t value = ssd->readLBA(lba);
	std::ostringstream oss;
	oss << "0x" << std::setfill('0') << std::setw(8)
		<< std::hex << std::uppercase << value;
	outputFile->saveToFile(oss.str());
}

void Command::executeErase(int lba, int size) {
	if (lba < 0 || size <= 0 || size > 10 || (lba + size) > 100) {
		outputFile->saveToFile("ERROR");
		return;
	}

	for (int i = 0; i < size; ++i) {
		ssd->writeLBA(lba + i, 0x00000000);
	}
}

