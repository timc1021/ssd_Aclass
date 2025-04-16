#include <iostream>
#include "TestShell.h"

vector<string> TestShell::splitBySpace(const string& input) {
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

unsigned int TestShell::init(string input) {
    vector<string> result = splitBySpace(input);

    if (result[0] == "read") {
        // call read()
        return true;
    }
    else if (result[0] == "write") {
        // call write()
        return true;
    }
}
int TestShell::write(int lba, uint32_t data)
{
	int result = 0; // system("ssd.exe");

	if (result == 0) {
		std::cout << "write done, lba : " << lba << ", data : " << data << std::endl;
	}
	else {
		return -1;
	}
	return result;
}

int TestShell::fullWrite(uint32_t data)
{
	int result = 0;

	for (int i = 0; i < 100; i++) {
		result = write(i, data);
		if (result != 0) {
			return -1;
		}
	}
	return 0;
}


void TestShell::exit() {
	exit();
}

int TestShell::read(int lba)
{
	int result = 0;
	int read_data = 0;
	// result = system("ssd.exe");

	if (result == 0) {
		std::cout << "read done, lba : " << lba << ", data : " << read_data << std::endl;
	}
	else {
		return -1;
	}
	return read_data;
}

int TestShell::fullRead()
{
	int result = 0;
	int read_data = 0;

	for (int lba = 0; lba < 100; lba++) {
		read_data = TestShell::read(lba);
	}

	if (result == 0) {
		std::cout << "full read done\n";
	}
	else {
		return -1;
	}

	return result;
}

int TestShell::help()
{
	int result = 0;

	std::cout << "help done\n";

	return result;
}
