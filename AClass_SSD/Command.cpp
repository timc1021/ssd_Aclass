#include "Command.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

Command::Command(std::shared_ptr<SSDControllerInterface> ssd,
    std::shared_ptr<FileTextIOInterface> outputFile,
    std::shared_ptr<CommandBuffer> buffer)
    : ssd(ssd), outputFile(outputFile), buffer(buffer) {
    registerCommands();
}

void Command::registerCommands() {
    commandMap["W"] = [this](int lba, const std::string& valueHex) {
        if (valueHex.length() != 10 || valueHex.substr(0, 2) != "0X")
            throw std::invalid_argument("Invalid hex format. Must be '0x' followed by 8 hex digits.");

        buffer->addCommandToBuffer(CommandValue("W " + std::to_string(lba) + " " + valueHex));
        };

    commandMap["E"] = [this](int lba, const std::string& valueHex) {
        int size = std::stoi(valueHex);
        if (size < 1 || size > 10)
            throw std::invalid_argument("Invalid erase size. Must be between 1 and 10.");

        buffer->addCommandToBuffer(CommandValue("E " + std::to_string(lba) + " " + std::to_string(size)));
        };

    commandMap["F"] = [this](int, const std::string&) {
        buffer->flush();
        };

    commandMap["R"] = [this](int lba, const std::string&) {
        uint32_t value;
        if (!buffer->getBufferedValueIfExists(lba, value)) {
            value = ssd->readLBA(lba);
        }

        std::ostringstream oss;
        oss << "0x" << std::setfill('0') << std::setw(8)
            << std::hex << std::uppercase << value;

        outputFile->saveToFile(oss.str());
        };
}

void Command::execute(const std::string& cmdTypeRaw, int lba, const std::string& valueHexRaw) {
    std::string cmdType = cmdTypeRaw;
    std::transform(cmdType.begin(), cmdType.end(), cmdType.begin(), ::toupper);

    std::string valueHex = valueHexRaw;
    std::transform(valueHex.begin(), valueHex.end(), valueHex.begin(), ::toupper);

    auto it = commandMap.find(cmdType);
    if (it != commandMap.end()) {
        it->second(lba, valueHex);
    }
    else {
        throw std::invalid_argument("Invalid command. Use 'R', 'W', 'E', or 'F'.");
    }
}
