#include "Command.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

Command::Command(SSDControllerInterface& ssdData, DataInterface& outputFile)
    : ssd(ssdData), outputFile(outputFile) {}

void Command::execute(const std::string& cmdType, int lba, const std::string& valueHex) {
    if (cmdType == "W") {
        if (valueHex.length() != 10 || valueHex.substr(0, 2) != "0x")
            throw std::invalid_argument("Invalid hex format. Must be '0x' followed by 8 hex digits.");

        uint32_t value = std::stoul(valueHex, nullptr, 16);
        ssd.writeLBA(lba, value);  // LBA에 값을 쓰기
    }
    else if (cmdType == "R") {
        uint32_t value = ssd.readLBA(lba);  // LBA에서 값 읽기

        // 0x00000000 형태로 저장
        std::ostringstream oss;
        oss << "0x" << std::setfill('0') << std::setw(8) << std::hex << std::uppercase << value;
        outputFile.saveToFile(oss.str());  // 읽은 값을 출력 파일에 저장
    }
    else {
        throw std::invalid_argument("Invalid command. Use 'R' or 'W'.");
    }
}
