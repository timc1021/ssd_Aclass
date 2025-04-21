#include "Command.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

Command::Command(std::shared_ptr<SSDControllerInterface> ssd,
	std::shared_ptr<FileTextIOInterface> outputFile,
	std::shared_ptr<CommandBuffer> buffer)
	: ssd(ssd), outputFile(outputFile), buffer(buffer) {}

void Command::execute(const std::string& cmdTypeRaw, int lba, const std::string& valueHexRaw) {
	std::string cmdType = cmdTypeRaw;
	std::transform(cmdType.begin(), cmdType.end(), cmdType.begin(), ::toupper);

	std::string valueHex = valueHexRaw;
	std::transform(valueHex.begin(), valueHex.end(), valueHex.begin(), ::toupper);

	if (cmdType == "W") {
		if (valueHex.length() != 10 || valueHex.substr(0, 2) != "0X") {
			throw std::invalid_argument("Invalid hex format. Must be '0x' followed by 8 hex digits.");
		}

		std::string fullCmd = "W " + std::to_string(lba) + " " + valueHex;
		buffer->addCommandToBuffer(CommandValue(fullCmd));
	}
	else if (cmdType == "E") {
		int size = std::stoi(valueHex);
		if (size < 1 || size > 10) {
			throw std::invalid_argument("Invalid erase size. Must be between 1 and 10.");
		}

		std::string fullCmd = "E " + std::to_string(lba) + " " + std::to_string(size);
		buffer->addCommandToBuffer(CommandValue(fullCmd));
	}
	else if (cmdType == "F") {
		buffer->flush();
	}
	else if (cmdType == "R") {
		uint32_t value;
		if (buffer->getBufferedValueIfExists(lba, value)) {
			// Fast Read
		}
		else {
			value = ssd->readLBA(lba);
		}

		std::ostringstream oss;
		oss << "0x" << std::setfill('0') << std::setw(8)
			<< std::hex << std::uppercase << value;

		outputFile->saveToFile(oss.str());
	}
	else {
		throw std::invalid_argument("Invalid command. Use 'R', 'W', 'E', or 'F'.");
	}
}
