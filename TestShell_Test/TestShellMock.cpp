#include "TestShellMock.h"

COMMAND_RESULT TestShellMock::handleCommand(string commandLine) {
    vector<string> commandToken = splitBySpace(commandLine);

    if (commandToken[0] == "read") {
        if (commandToken.size() != 2)
            return COMMAND_INVALID_PARAM;
        if (std::stoi(commandToken[1]) >= 100 || std::stoi(commandToken[1]) < 0)
            return COMMAND_INVALID_PARAM;

        read(std::stoi(commandToken[1]));
    }
    else if (commandToken[0] == "write") {
        if (commandToken.size() != 3)
            return COMMAND_INVALID_PARAM;
        if (std::stoi(commandToken[1]) >= 100 || std::stoi(commandToken[1]) < 0)
            return COMMAND_INVALID_PARAM;
        if (commandToken[2].length() != 10)
            return COMMAND_INVALID_PARAM;

        // check the data input starts with "0x"
        if (commandToken[2].substr(0, 2) != "0x") {
            return COMMAND_INVALID_PARAM;
        }

        // check the data range after "0x". 
        for (size_t i = 2; i < commandToken[2].length(); ++i) {
            char c = commandToken[2][i];
            // should be one of those "A~F", "0~9"
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
                return COMMAND_INVALID_PARAM;
            }
        }

        write(std::stoi(commandToken[1]), static_cast<unsigned int>(std::stoul(commandToken[2], nullptr, 16)));
    }
    else if (commandToken[0] == "fullRead") {
        fullRead();
    }
    else if (commandToken[0] == "fullWrite") {
        fullWrite(0xAAAABBBB); // TODO
    }
    else if (commandToken[0] == "help") {
        help();
    }
    else if (commandToken[0] == "exit") {
        return exit();
    }
    else {
        return COMMAND_INVALID_PARAM;
    }
    return COMMAND_SUCCESS;
}

vector<string> TestShellMock::splitBySpace(const string& input) {
    std::vector<std::string> tokens;
    size_t start = 0, end;

    while ((end = input.find(' ', start)) != string::npos) {
        if (end != start)
            tokens.push_back(input.substr(start, end - start));
        start = end + 1;
    }

    if (start < input.size())
        tokens.push_back(input.substr(start));

    return tokens;
}

void TestShellMock::fullWrite(uint32_t data)
{
    for (int lba = 0; lba < 100; lba++) {
        write(lba, data);
    }
}

uint32_t TestShellMock::fullRead()
{
    int read_data = 0;

    for (int lba = 0; lba < 100; lba++) {
        read_data = read(lba);
    }

    return read_data;
}

COMMAND_RESULT TestShellMock::exit()
{
    return COMMAND_EXIT;
}

void TestShellMock::help()
{
    std::cout << "help\n"; // TODO
}
