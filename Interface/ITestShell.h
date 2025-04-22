#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "ICommand.h"
#include "Type.h"

using namespace std;

#define MAX_ERASE_LBA (10)
#define START_LBA     (0)
#define ADD_LOG		  (Logger::getInstance().addLog)

class ITestScript;
class ITestShell;

class ITestShell {
public:
	virtual void write(const int lba, const uint32_t data) = 0;
	virtual uint32_t read(const int lba) = 0;
	virtual void erase(const int lba, const int size) = 0;
	virtual void registerCommand(const string& command, ITestScript* script) = 0;
	virtual bool readCompareRange(int start_lba, int end_lba, uint32_t data) = 0;
	virtual void writeLBAs(const vector<int>lba, const uint32_t data) = 0;
	virtual bool readCompare(const int lba, const uint32_t expected) = 0;
	virtual void flush() = 0;

	void initCommands();
	COMMAND_RESULT handleCommand(const string& commandLine);
	COMMAND_RESULT executeShellCommand(const vector<string> commandToken);
	COMMAND_RESULT executeTestScript(const string& tcName);
	bool IsShellCommand(const vector<string> commandToken);
	bool IsExistTestScript(const string& command);
	vector<string> splitBySpace(const string& input);
	void setScript(const string& command, ITestScript* script);

protected:
	unordered_map<string, ITestScript*> testScriptCommand;
	unordered_map<string, ICommand*> commandMap;
};