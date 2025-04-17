#include <iostream>
#include <string>
#include "Command.h"
#include "SSDController.h"
#include "FileTextIO.h"

int main(int argc, char* argv[]) {

    std::string cmdType = argv[1];
    int lba = std::stoi(argv[2]);
    std::string value = (cmdType == "W" && argc == 4) ? argv[3] : "";

    std::shared_ptr<SSDController> ssd = std::make_shared<SSDController>(std::make_shared< FileTextIO>("ssd_nand.txt"));
    std::shared_ptr <FileTextIO> outputFile = std::make_shared<FileTextIO>("ssd_output.txt");
    Command command(ssd, outputFile);

    try {
        command.execute(cmdType, lba, value);
    }
    catch (const std::exception& e) {
        outputFile->saveToFile("ERROR");
    }
    return 0;
}
