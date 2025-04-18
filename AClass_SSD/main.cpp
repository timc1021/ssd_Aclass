#include <iostream>
#include <string>
#include <memory> 
#include "Command.h"
#include "SSDController.h"
#include "FileTextIO.h"

int main(int argc, char* argv[]) {
	std::string cmdType = argv[1];
	int lba = std::stoi(argv[2]);

	std::string value = (argc == 4) ? argv[3] : "";

	auto nandFile = std::make_shared<FileTextIO>("ssd_nand.txt");
	auto outputFile = std::make_shared<FileTextIO>("ssd_output.txt");
	auto ssd = std::make_shared<SSDController>(nandFile);
	Command command(ssd, outputFile);

	try {
		command.execute(cmdType, lba, value);
	}
	catch (const std::exception& e) {
		outputFile->saveToFile("ERROR");
	}

	return 0;
}
