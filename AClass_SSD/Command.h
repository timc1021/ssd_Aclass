#pragma once
#include <memory>
#include "SSDControllerInterface.h"
#include "FileTextIOInterface.h"

class Command {
public:
	Command(std::shared_ptr<SSDControllerInterface> ssdData,
		std::shared_ptr<FileTextIOInterface> outputFile);

	void execute(const std::string& cmdType, int lba, const std::string& valueHex = "");

private:
	void executeWrite(int lba, const std::string& valueHex);
	void executeRead(int lba);

	std::shared_ptr<SSDControllerInterface> ssd;
	std::shared_ptr<FileTextIOInterface> outputFile;
};
