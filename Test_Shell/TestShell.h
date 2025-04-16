#pragma once
#include <vector>

using std::string;
using std::vector;

class TestShell {
public:
	unsigned int init(string input);
	vector<string> splitBySpace(const string& input);
	int write(int lba, uint32_t data);
	int read(int lba);
	int fullWrite(uint32_t data);
	int fullRead();
	int help();
	void exit();
};