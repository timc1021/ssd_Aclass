#pragma once
#include "ITestShell.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class TestShell : public ITestShell {
public:
	COMMAND_RESULT handleCommand(string input);
	vector<string> splitBySpace(const string& input);
	void write(int lba, uint32_t data);
	uint32_t read(int lba);
	void fullWrite(uint32_t data);
	uint32_t fullRead();
	void help();
	COMMAND_RESULT exit();
};