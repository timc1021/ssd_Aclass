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
	NOT_A_DEFAULT_COMMAND,
} COMMAND_RESULT;

class ITestScript;
class ITestShell;

class ITestCommand {
public:
	virtual ~ITestCommand() {}
	virtual std::vector<std::string> names() const = 0;
	virtual COMMAND_RESULT *run(const vector<string> commandToken) = 0;
	virtual void setShell(ITestShell* shell) = 0;

	ITestShell* shell = nullptr;
};

class ITestShell {
public:
	using FuncPtr = COMMAND_RESULT(ITestShell::*)(const vector<string> commandToken);

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
	bool isExistTestScript(const string& command);
	vector<string> splitBySpace(const string& input);
	void fullWrite(const uint32_t data);
	uint32_t fullRead();
	COMMAND_RESULT help(const vector<string> commandToken);
	COMMAND_RESULT exit(const vector<string> commandToken);
	COMMAND_RESULT handleRead(const vector<string> commandToken);
	COMMAND_RESULT handleFullread(const vector<string> commandToken);
	COMMAND_RESULT handleWrite(const vector<string> commandToken);
	COMMAND_RESULT handleFullwrite(const vector<string> commandToken);
	COMMAND_RESULT handleErase(const vector<string> commandToken);
	COMMAND_RESULT handleEraseRange(const vector<string> commandToken);
	COMMAND_RESULT handleFlush(const vector<string> commandToken);

	void setScript(const string& command, ITestScript* script);

	ITestShell() {
		testCommand["read"] = &ITestShell::handleRead;
		testCommand["write"] = &ITestShell::handleWrite;
		testCommand["fullread"] = &ITestShell::handleFullread;
		testCommand["fullwrite"] = &ITestShell::handleFullwrite;
		testCommand["erase"] = &ITestShell::handleErase;
		testCommand["erase_range"] = &ITestShell::handleEraseRange;
		testCommand["flush"] = &ITestShell::handleFlush;
		testCommand["help"] = &ITestShell::help;
		testCommand["exit"] = &ITestShell::exit;
	}

	COMMAND_RESULT CallCommand(const string& key, const vector<string> commandToken) {
		COMMAND_RESULT result = COMMAND_INVALID_PARAM;
		auto it = testCommand.find(key);
		if (it != testCommand.end()) {
			FuncPtr func = it->second;
			result = (this->*func)(commandToken);  // 인자를 전달하며 멤버 함수 호출
		}
		else {
			result = NOT_A_DEFAULT_COMMAND;
		}
		return result;
	}

private:
	bool isWriteDataValid(const string& commandLine);
	bool isWriteCommandValid(const vector<string> commandToken);
	bool isReadCommandValid(const vector<string> commandToken);
	bool isFullwriteCommandValid(const vector<string> commandToken);
	bool isLBAValid(const string& lba);
	bool isEraseCommandValid(const vector<string> commandToken);
	bool isEraseRangeCommandValid(const vector<string> commandToken);
	bool isFlushCommandValid(const vector<string> commandToken);
	bool isValidInteger(const string& str);
	bool isValidUnsignedLong(const string& str);

	unordered_map<string, FuncPtr> testCommand;
	unordered_map<string, ITestScript*> testScriptCommand;
};