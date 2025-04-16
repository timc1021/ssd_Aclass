#pragma once
#include "ITestShell.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class TestShell : public ITestShell {
public:
	int handleCommand(string input);
	vector<string> splitBySpace(const string& input);
	int write(int lba, uint32_t data);
	uint32_t read(int lba);
	int fullWrite(uint32_t data);
	uint32_t fullRead();
	void help();
	int exit();
};