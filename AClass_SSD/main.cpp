#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

#include "FileTextIO.h"
#include "SSDController.h"
#include "CommandBuffer.h"
#include "Command.h"

std::shared_ptr<ICommandExecutor> createCommandExecutor(
	std::shared_ptr<SSDControllerInterface> ssd,
	std::shared_ptr<FileTextIOInterface> output,
	std::shared_ptr<CommandBuffer> buffer)
{
	return std::make_shared<Command>(ssd, output, buffer);
}

int main(int argc, char* argv[]) {
	auto nand = std::make_shared<FileTextIO>("ssd_nand.txt");
	auto output = std::make_shared<FileTextIO>("ssd_output.txt");
	auto ssd = std::make_shared<SSDController>(nand);
	auto buffer = std::make_shared<CommandBuffer>(ssd);
	auto executor = createCommandExecutor(ssd, output, buffer);

	try {
		if (argc < 2) {
			throw std::invalid_argument("비정상 입력입니다.");
		}

		std::string cmdType = argv[1];
		std::transform(cmdType.begin(), cmdType.end(), cmdType.begin(), ::toupper);

		int lba = 0;
		std::string value = "";

		if (cmdType == "W" || cmdType == "E" || cmdType == "R") {
			if (argc < 3) {
				throw std::invalid_argument("비정상 입력입니다.");
			}
			lba = std::stoi(argv[2]);
		}

		if (cmdType == "W" || cmdType == "E") {
			if (argc < 4) {
				throw std::invalid_argument("비정상 입력입니다.");
			}
			value = argv[3];
			std::transform(value.begin(), value.end(), value.begin(), ::toupper);
		}

		executor->execute(cmdType, lba, value);
	}
	catch (const std::exception& e) {
		output->saveToFile("ERROR");
	}

	return 0;
}