#include "TestShellMock.h"

int TestShellMock::handleCommand(string input) {
    vector<string> result = splitBySpace(input);

    if (result[0] == "read") {
        read(0); // TODO
    }
    else if (result[0] == "write") {
        write(3, 0xAAAABBBB); // TODO
    }
    else if (result[0] == "fullRead") {
        fullRead();
    }
    else if (result[0] == "fullWrite") {
        fullWrite(0xAAAABBBB); // TODO
    }
    else if (result[0] == "help") {
        help();
    }
    else if (result[0] == "exit") {
        return exit();
    }
    else {
        // invalid command
        return -1;
    }
    return 0;
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

int TestShellMock::fullWrite(uint32_t data)
{
    int result;

    for (int lba = 0; lba < 100; lba++) {
        result = write(lba, data);
    }
    
    return result;
}

uint32_t TestShellMock::fullRead()
{
    int read_data;

    for (int lba = 0; lba < 100; lba++) {
        read_data = read(lba);
    }

    return 0; // TODO
}

int TestShellMock::exit()
{
    return -2;
}

void TestShellMock::help()
{
    std::cout << "help\n"; // TODO
}
