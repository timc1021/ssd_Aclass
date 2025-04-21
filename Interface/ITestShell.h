#pragma once
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

#define MAX_ERASE_LBA (10)
#define MAX_LBA_SIZE  (100)
#define START_LBA     (0)
#define ADD_LOG		  (Logger::getInstance().addLog)

typedef enum {
	COMMAND_SUCCESS,
	COMMAND_INVALID_PARAM,
	COMMAND_TC_FAIL,
	COMMAND_EXIT,
} COMMAND_RESULT;

class ITestScript;

class ITestShell {
public:
	virtual void write(const int lba, const uint32_t data) = 0;
	virtual uint32_t read(const int lba) = 0;
	virtual void erase(const int lba, const int size) = 0;
	virtual void registerCommand(const string& command, ITestScript* script) = 0;
	virtual bool readCompareRange(int start_lba, int end_lba, uint32_t data) = 0;
	virtual void writeLBAs(const vector<int>lba, const uint32_t data) = 0;
	virtual bool readCompare(const int lba, const uint32_t expected) = 0;
	virtual void eraseRange(const int startLba, const int endLba) = 0;
	virtual void flush() = 0;

	COMMAND_RESULT handleCommand(const string& commandLine);
	COMMAND_RESULT handleShellCommand(const vector<string> commandToken);
	COMMAND_RESULT handleTestScript(const string& tcName);
	bool IsExistTestScript(const string& command);
	vector<string> splitBySpace(const string& input);
	void fullWrite(const uint32_t data);
	uint32_t fullRead();
	void help();
	COMMAND_RESULT exit();
	COMMAND_RESULT handleRead(const vector<string> commandToken);
	COMMAND_RESULT handleFullread(void);
	COMMAND_RESULT handleWrite(const vector<string> commandToken);
	COMMAND_RESULT handleFullwrite(const vector<string> commandToken);
	COMMAND_RESULT handleErase(const vector<string> commandToken);
	COMMAND_RESULT handleEraseRange(const vector<string> commandToken);
	COMMAND_RESULT handleFlush(const vector<string> commandToken);

	void setScript(const string& command, ITestScript* script);

private:
	bool isWriteDataValid(const string& commandLine);
	bool isWriteCommandValid(const vector<string> commandToken);
	bool isReadCommandValid(const vector<string> commandToken);
	bool isFullwriteCommandValid(const vector<string> commandToken);
	bool isLBAValid(const string& lba);
	bool isCommandValid(const vector<string> commandToken);
	bool isEraseCommandValid(const vector<string> commandToken);
	bool isEraseRangeCommandValid(const vector<string> commandToken);
	bool isFlushCommandValid(const vector<string> commandToken);

	unordered_map<string, ITestScript*> testScriptCommand;
	const vector<string> commandList = {
	"read",
	"write",
	"fullread",
	"fullwrite",
	"erase",
	"erase_range",
	"flush",
	"help",
	"exit",
	};
};