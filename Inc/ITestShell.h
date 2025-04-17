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
	COMMAND_RESULT handleCommand(const string& commandLine);
	vector<string> splitBySpace(const string& input);
	void fullWrite(uint32_t data);
	uint32_t fullRead();
	void help();
	COMMAND_RESULT exit();

	bool readCompare(int lba, uint32_t expected);
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();

	virtual void write(int lba, uint32_t data) = 0;
	virtual uint32_t read(int lba) = 0;

	const int MAX_LBA_SIZE = 100;

private:
	bool isCapitalLetter(const char c);
	bool isWriteDataValid(const string& commandLine);
	bool isWriteCommandValid(const string& commandLine);
	bool isReadCommandValid(const string& commandLine);
	bool isValidLBA(const vector<string>& commandToken);
	bool isCommandValid(const string& commandLine);
};