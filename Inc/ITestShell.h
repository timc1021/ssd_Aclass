#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

typedef enum {
	COMMAND_SUCCESS,
	COMMAND_INVALID_PARAM,
	COMMAND_EXIT,
} COMMAND_RESULT;

class ITestShell {
public:
	virtual COMMAND_RESULT handleCommand(string commandLine) = 0;
	virtual void write(int lba, uint32_t data) = 0;
	virtual void fullWrite(uint32_t data) = 0;
	virtual uint32_t read(int lba) = 0;
	virtual uint32_t fullRead() = 0;
	virtual void help() = 0;
	virtual COMMAND_RESULT exit() = 0;
};