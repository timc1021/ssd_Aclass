#include <iostream>
#include <string>
#include "Command.h"
#include "SSDController.h"
#include "FileTextIO.h"

int main(int argc, char* argv[]) {

    std::string cmdType = argv[1];
    int lba = std::stoi(argv[2]);
    std::string value = (cmdType == "W" && argc == 4) ? argv[3] : "";

    FileTextIO nandFile("ssd_nand.txt");
    FileTextIO outputFile("ssd_output.txt");
    SSDController ssd(&nandFile);
    Command command(ssd, outputFile);
    try {
        command.execute(cmdType, lba, value);
    }
    catch (const std::exception& e) {
        outputFile.saveToFile("ERROR");
    }
    return 0;
}
