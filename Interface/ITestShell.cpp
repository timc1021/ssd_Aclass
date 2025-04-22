#include <iostream>
#include <iomanip>
#include <sstream>
#include "ITestShell.h"
#include "ITestScript.h"
#include "Logger.h"
#include "ReadCommand.h"
#include "WriteCommand.h"
#include "FullReadCommand.h"
#include "FullWriteCommand.h"
#include "FlushCommand.h"
#include "EraseCommand.h"
#include "EraseRangeCommand.h"
#include "ExitCommand.h"
#include "HelpCommand.h"

#define HEX_PREFIX			("0x")
#define HEX_PREFIX_LENGTH	(2)
#define TOKEN_COMMAND		(0)
#define DATA_LENGTH         (10)


void ITestShell::initCommands()
{
	commandMap["read"] = new ReadCommand(this);
	commandMap["write"] = new WriteCommand(this);
	commandMap["fullwrite"] = new FullWriteCommand(this);
	commandMap["fullread"] = new FullReadCommand(this);
	commandMap["flush"] = new FlushCommand(this);
	commandMap["exit"] = new ExitCommand(this);
	commandMap["help"] = new HelpCommand(this);
	commandMap["erase"] = new EraseCommand(this);
	commandMap["erase_range"] = new EraseRangeCommand(this);
}

COMMAND_RESULT ITestShell::handleCommand(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);
	string command = commandToken[TOKEN_COMMAND];
	COMMAND_RESULT result = COMMAND_SUCCESS;

	if (IsShellCommand(commandToken))
	{
		result = executeShellCommand(commandToken);
	}
	else if (IsExistTestScript(command))
	{
		result = executeTestScript(command);
	}
	else
	{
		ADD_LOG("ITestShell::handleCommand", "ERROR, Invalid Command");
		result = COMMAND_INVALID_PARAM;
	}
	return result;
}

COMMAND_RESULT ITestShell::executeShellCommand(const vector<string> commandToken)
{
	const string& commandName = commandToken[TOKEN_COMMAND];
	auto it = commandMap.find(commandName);
	ICommand* command = it->second;
	return command->execute(commandToken);
}

bool ITestShell::IsShellCommand(const vector<string> commandToken)
{
	if (commandToken.empty()) {
		return false;;
	}

	const string& commandName = commandToken[TOKEN_COMMAND];
	auto it = commandMap.find(commandName);
	if (it == commandMap.end()) {
		return false;
	}

	return true;
}

COMMAND_RESULT ITestShell::executeTestScript(const string& tcName)
{
	auto it = testScriptCommand.find(tcName);
	if (it == testScriptCommand.end()) {
		return COMMAND_INVALID_PARAM;
	}

	ITestScript* script = it->second;

	bool result = script->run();

	string msg = (result == true) ? "PASS" : "FAIL";
	ADD_LOG("ITestShell::handleTestScript", msg);

	if (result == false) {
		return COMMAND_TC_FAIL;
	}

	return COMMAND_SUCCESS;
}

bool ITestShell::IsExistTestScript(const string& command)
{
	if (testScriptCommand.find(command) != testScriptCommand.end()) {
		return true;
	}
	return false;
}

vector<string> ITestShell::splitBySpace(const string& input) {
	vector<string> tokens;
	size_t start = 0, end;

	while ((end = input.find(' ', start)) != string::npos) {
		if (end != start)
			tokens.push_back(input.substr(start, end - start));
		start = end + 1;
	}

	if (start < input.size())
		tokens.push_back(input.substr(start));

	return tokens;
}

void ITestShell::setScript(const string& command, ITestScript* script)
{
	testScriptCommand[command] = script;
}