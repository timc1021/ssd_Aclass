#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

#include "FileTextIOInterface.h"
#include "FileTextIO.h"
#include "SSDController.h"
#include "CommandBuffer.h"
#include "Command.h"

int main(int argc, char* argv[]) {
	std::string cmdType = argv[1];
	std::transform(cmdType.begin(), cmdType.end(), cmdType.begin(), ::toupper);

	int lba = argc > 2 ? std::stoi(argv[2]) : 0;

	std::string value = (argc == 4) ? argv[3] : "";
	std::transform(value.begin(), value.end(), value.begin(), ::toupper);

	std::shared_ptr<FileTextIOInterface> nand = std::make_shared<FileTextIO>("ssd_nand.txt");
	std::shared_ptr<FileTextIOInterface> output = std::make_shared<FileTextIO>("ssd_output.txt");
	std::shared_ptr<SSDControllerInterface> ssd = std::make_shared<SSDController>(nand);
	std::shared_ptr<CommandBuffer> buffer = std::make_shared<CommandBuffer>(ssd);

	Command command(ssd, output, buffer);

	try {
		command.execute(cmdType, lba, value);
	}
	catch (const std::exception& e) {
		output->saveToFile("ERROR");
	}

	return 0;
}
