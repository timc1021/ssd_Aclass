#pragma once
#include <memory>
#include <string>
#include "SSDControllerInterface.h"
#include "FileTextIOInterface.h"
#include "CommandBuffer.h"

class Command {
private:
	std::shared_ptr<SSDControllerInterface> ssd;
	std::shared_ptr<FileTextIOInterface> outputFile;
	std::shared_ptr<CommandBuffer> buffer;

public:
	Command(std::shared_ptr<SSDControllerInterface> ssd,
		std::shared_ptr<FileTextIOInterface> outputFile,
		std::shared_ptr<CommandBuffer> buffer);

	void execute(const std::string& cmdType, int lba, const std::string& valueHex = "");
};
