#include "Command.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

Command::Command(std::shared_ptr<SSDControllerInterface> ssdData, std::shared_ptr<FileTextIOInterface> outputFile)
	: ssd(std::move(ssdData)), outputFile(std::move(outputFile)){}

void Command::execute(const std::string& cmdType, int lba, const std::string& valueHex) {
	if (cmdType == "W") {
		if (valueHex.length() != 10 || valueHex.substr(0, 2) != "0x")
			throw std::invalid_argument("Invalid hex format. Must be '0x' followed by 8 hex digits.");

		uint32_t value = std::stoul(valueHex, nullptr, 16);
		ssd->writeLBA(lba, value);
	}
	else if (cmdType == "R") {
		uint32_t value = ssd->readLBA(lba);

		std::ostringstream oss;
		oss << "0x" << std::setfill('0') << std::setw(8) << std::hex << std::uppercase << value;
		outputFile->saveToFile(oss.str());
	}
	else {
		throw std::invalid_argument("Invalid command. Use 'R' or 'W'.");
	}
}
