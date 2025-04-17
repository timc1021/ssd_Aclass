#pragma once
#include "SSDControllerInterface.h"

class Command {
private:
    SSDControllerInterface& ssd;
    FileTextIOInterface& outputFile;

public:
    Command(SSDControllerInterface& ssdData, FileTextIOInterface& outputFile);
    void execute(const std::string& cmdType, int lba, const std::string& valueHex = "");
};
