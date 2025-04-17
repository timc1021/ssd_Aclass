#pragma once
#include "SSDControllerInterface.h"

class Command {
private:
	std::shared_ptr<SSDControllerInterface> ssd;
	std::shared_ptr<FileTextIOInterface> outputFile;

public:
	Command(std::shared_ptr<SSDControllerInterface> ssdData, std::shared_ptr<FileTextIOInterface> outputFile);
	void execute(const std::string& cmdType, int lba, const std::string& valueHex = "");
};
