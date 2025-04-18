#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <string>
#include <vector>

#include "TestShellApp.h"
#include "ITestScript.h"
#include "Logger.h"

void TestShellApp::run(std::istream& in, std::ostream& out)
{
    init();

    while (true) {
        std::string command;

        out << "shell> ";

        if (!std::getline(in, command)) break;
        if (command.empty()) continue;

        COMMAND_RESULT result = testShell->handleCommand(command);
        if (result == COMMAND_INVALID_PARAM)
        {
            out << "INVALID COMMAND\n";
        }
        else if (result == COMMAND_EXIT)
        {
            break;
        }

        out << "\n";
    }
}

void TestShellApp::init()
{
    Logger::getInstance().initLogFile();

    for (const auto& entry : std::filesystem::directory_iterator("./scripts")) {
        if (entry.path().extension() == ".dll") {
            HMODULE dll = LoadLibrary(entry.path().c_str());
            if (!dll) continue;

            using CreateFn = ITestScript * (*)();
            CreateFn create = (CreateFn)GetProcAddress(dll, "create");
            if (!create) continue;

            ITestScript* script = create();
            for (const std::string& cmd : script->names()) {
                testShell->registerCommand(cmd, script);  // 명령어 등록
            }
        }
    }
}

void TestShellApp::runner(char* argv)
{
    bool result = false;
    std::string command;
    std::string file_name = argv;

    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << file_name << std::endl;
        return;
    }

    while(!file.eof()) {
        getline(file, command);
        std::cout << command << "---   RUN   ...";

        std::streambuf* originalCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(nullptr);

        if (command == "1_FullWriteAndReadCompare" || command == "1_") {
            result = testShell->fullWriteAndReadCompare();
        }
        else if (command == "2_PartialLBAWrite" || command == "2_") {
            result = testShell->partialLBAWrite();
        }
        else if (command == "3_WriteReadAging" || command == "3_") {
            result = testShell->writeReadAging();
        }
        else if (command == "4_EraseAndWriteAging" || command == "4_") {
            result = testShell->eraseAndWriteAging();
        }
        std::cout.rdbuf(originalCoutBuffer);

        if (result == true) {
            std::cout << "Pass\n";
        }
        else {
            std::cout << "Fail\n";
            file.close();
            return;
        }
    }

    file.close();
}
