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
	uint32_t fullRead();
	void help();
	COMMAND_RESULT exit();

	virtual void write(int lba, uint32_t data) = 0;
	virtual uint32_t read(int lba) = 0;

	const int MAX_LBA_SIZE = 100;

private:
	bool isWriteDataValid(string& commandLine);
	bool isWriteCommandValid(string& commandLine);
	bool isReadCommandValid(string& commandLine);
	bool isValidLBA(vector<string>& commandToken);
	bool isCommandValid(string& commandLine);
};