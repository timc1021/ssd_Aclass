#include <iostream>
#include "TestShellApp.h"
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
}
