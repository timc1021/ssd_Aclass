#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class ITestShell {
public:
	virtual int handleCommand(string input) = 0;
	virtual int write(int lba, uint32_t data) = 0;
	virtual uint32_t read(int lba) = 0;
	virtual int fullWrite(uint32_t data) = 0;
	virtual uint32_t fullRead() = 0;
	virtual void help() = 0;
	virtual int exit() = 0;
};