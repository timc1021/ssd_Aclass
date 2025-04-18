#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

#define MAX_ERASE_LBA (10)
#define MAX_LBA_SIZE  (100)
#define START_LBA     (0)

typedef enum {
	COMMAND_SUCCESS,
	COMMAND_INVALID_PARAM,
	COMMAND_EXIT,
} COMMAND_RESULT;

class ITestShell {
public:
	COMMAND_RESULT handleCommand(const string& commandLine);
	vector<string> splitBySpace(const string& input);
	void fullWrite(const uint32_t data);
	uint32_t fullRead();
	void help();
	COMMAND_RESULT exit();

	bool readCompare(const int lba, const uint32_t expected);
	bool fullWriteAndReadCompare();
	bool partialLBAWrite();
	bool writeReadAging();
	bool eraseAndWriteAging();

	virtual void write(const int lba, const uint32_t data) = 0;
	virtual uint32_t read(const int lba) = 0;
	virtual void erase(const int lba, const int size) = 0;

private:
	bool isWriteDataValid(const string& commandLine);
	bool isWriteCommandValid(const vector<string> commandToken);
	bool isReadCommandValid(const vector<string> commandToken);
	bool isFullwriteCommandValid(const vector<string> commandToken);
	bool isLBAValid(const string& lba);
	bool isCommandValid(const vector<string> commandToken);
	bool isEraseCommandValid(const vector<string> commandToken);
	bool isEraseRangeCommandValid(const vector<string> commandToken);
	bool readCompareRange(int start_lba, int end_lba, uint32_t data);
	void writeLBAs(const vector<int>lba, uint32_t data);

	COMMAND_RESULT handleRead(const vector<string> commandToken);
	COMMAND_RESULT handleFullread(void);
	COMMAND_RESULT handleWrite(const vector<string> commandToken);
	COMMAND_RESULT handleFullwrite(const vector<string> commandToken);
	COMMAND_RESULT handleErase(const vector<string> commandToken);
	COMMAND_RESULT handleEraseRange(const vector<string> commandToken);

	const vector<string> commandList = {
	"read",
	"write",
	"fullread",
	"fullwrite",
	"erase",
	"erase_range",
	"help",
	"exit",
	"1_FullWriteAndReadCompare",
	"1_",
	"2_PartialLBAWrite",
	"2_",
	"3_WriteReadAging",
	"3_",
	"4_EraseAndWriteAging",
	"4_",
	};
};