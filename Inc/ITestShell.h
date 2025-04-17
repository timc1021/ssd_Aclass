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
	COMMAND_RESULT handleCommand(string commandLine);
	vector<string> splitBySpace(const string& input);
	void fullWrite(uint32_t data);
	COMMAND_RESULT exit();
	uint32_t fullRead();
	void help();

	bool readCompare(int lba, uint32_t expected);
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();

	virtual void write(int lba, uint32_t data) = 0;
	virtual uint32_t read(int lb) = 0;
};