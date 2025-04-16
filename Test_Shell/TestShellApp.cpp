#include <iostream>
#include "TestShellApp.h"

void TestShellApp::run(std::istream& in, std::ostream& out)
{
    while (true) {
        std::string command;
        out << "shell > ";
        if (!(in >> command)) break;
        int result = testShell->handleCommand(command);
        if (result == -1)
        {
            out << "INVALID COMMAND\n";
        }
        else if (result == -2)
        {
            break;
        }
    }
}
