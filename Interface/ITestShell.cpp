#include <iostream>
#include <iomanip>
#include <sstream>
#include "ITestShell.h"
#include "ITestScript.h"
#include "Logger.h"

using std::cout;
using std::isdigit;
using std::stoi;

#define HEX_PREFIX			("0x")
#define HEX_PREFIX_LENGTH	(2)
#define TOKEN_COMMAND		(0)
#define DATA_LENGTH         (10)

typedef enum {
	TOKEN_WRITE_LBA = 1,
	TOKEN_WRITE_DATA,
	TOKEN_WRITE_NUM,
} WRITE_TOKEN;

typedef enum {
	TOKEN_FULLWRITE_DATA = 1,
	TOKEN_FULLWRITE_NUM,
} FULLWRITE_TOKEN;

typedef enum {
	TOKEN_READ_LBA = 1,
	TOKEN_READ_NUM,
} READ_TOKEN;

typedef enum {
	TOKEN_FULLREAD_NUM = 1,
} FULLREAD_TOKEN;

typedef enum {
	TOKEN_ERASE_LBA = 1,
	TOKEN_ERASE_SIZE,
	TOKEN_ERASE_NUM,
} ERASE_TOKEN;

typedef enum {
	TOKEN_FLUSH_NUM = 1,
} FLUSH_TOKEN;

typedef enum {
	TOKEN_ERASE_RANGE_START_LBA = 1,
	TOKEN_ERASE_RANGE_END_LBA,
	TOKEN_ERASE_RANGNE_NUM,
} ERASE_RANGE_TOKEN;

COMMAND_RESULT ITestShell::handleCommand(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);
	string command = commandToken[TOKEN_COMMAND];
	COMMAND_RESULT result = COMMAND_SUCCESS;

	result = handleShellCommand(commandToken);

	if (result == NOT_A_DEFAULT_COMMAND) {
		if (IsExistTestScript(command))
		{
			result = handleTestScript(command);
		}
		else
		{
			ADD_LOG("ITestShell::handleCommand", "ERROR, Invalid Command");
			result = COMMAND_INVALID_PARAM;
		}
	}

	return result;
}

COMMAND_RESULT ITestShell::handleShellCommand(const vector<string> commandToken)
{
	string command = commandToken[TOKEN_COMMAND];
	COMMAND_RESULT result = COMMAND_SUCCESS;

	result = CallCommand(command, commandToken);

	return result;
}

COMMAND_RESULT ITestShell::handleTestScript(const string& tcName)
{
	ITestScript* script = testScriptCommand.find(tcName)->second;
	bool result = script->run();

	if (result == false)
		return COMMAND_TC_FAIL;

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

bool ITestShell::isWriteDataValid(const string& writeData)
{
	if ((writeData.length() != DATA_LENGTH) ||
		(writeData.substr(0, HEX_PREFIX_LENGTH) != HEX_PREFIX)) {
		ADD_LOG("ITestShell::isWriteDataValid", "ERROR");

		return false;
	}

	for (char c : writeData.substr(HEX_PREFIX_LENGTH)) {
		if (isdigit(c) || isupper(c)) continue;

		ADD_LOG("ITestShell::isWriteDataValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isWriteCommandValid(const vector<string> commandToken) {

	if ((commandToken.size() != TOKEN_WRITE_NUM) ||
		(!isLBAValid(commandToken[TOKEN_WRITE_LBA])) ||
		(!isWriteDataValid(commandToken[TOKEN_WRITE_DATA]))) {
		ADD_LOG("ITestShell::isReadCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isReadCommandValid(const vector<string> commandToken) {
	if ((commandToken.size() != TOKEN_READ_NUM) ||
		(!isLBAValid(commandToken[TOKEN_READ_LBA]))) {
		ADD_LOG("ITestShell::isReadCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isFullwriteCommandValid(const vector<string> commandToken)
{
	if ((commandToken.size() != TOKEN_FULLWRITE_NUM) ||
		(!isWriteDataValid(commandToken[TOKEN_FULLWRITE_DATA]))) {
		ADD_LOG("ITestShell::isFullwriteCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isLBAValid(const string& lba)
{
	int iLba = std::stoi(lba);

	if (iLba >= MAX_LBA_SIZE || iLba < START_LBA) {
		ADD_LOG("ITestShell::isLBAValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isEraseCommandValid(const vector<string> commandToken)
{
	if ((commandToken.size() != TOKEN_ERASE_NUM) ||
		(!isLBAValid(commandToken[TOKEN_ERASE_LBA]))) {
		ADD_LOG("ITestShell::isEraseCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isEraseRangeCommandValid(const vector<string> commandToken)
{
	if ((commandToken.size() != TOKEN_ERASE_RANGNE_NUM) ||
		(!isLBAValid(commandToken[TOKEN_ERASE_RANGE_START_LBA])) ||
		(!isLBAValid(commandToken[TOKEN_ERASE_RANGE_END_LBA]))) {
		ADD_LOG("ITestShell::isEraseRangeCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isFlushCommandValid(const vector<string> commandToken)
{
	if (commandToken.size() != TOKEN_FLUSH_NUM)
		return false;

	return true;
}

COMMAND_RESULT ITestShell::handleRead(const vector<string> commandToken)
{
	if (!isReadCommandValid(commandToken))
		return COMMAND_INVALID_PARAM;

	read(stoi(commandToken[TOKEN_READ_LBA]));

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleFullread(const vector<string> commandToken)
{
	fullRead();

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleWrite(const vector<string> commandToken)
{
	if (!isWriteCommandValid(commandToken))
		return COMMAND_INVALID_PARAM;

	write(stoi(commandToken[TOKEN_WRITE_LBA]), static_cast<unsigned int>(std::stoul(commandToken[TOKEN_WRITE_DATA], nullptr, 0)));

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleFullwrite(const vector<string> commandToken)
{
	if (!isFullwriteCommandValid(commandToken))
		return COMMAND_INVALID_PARAM;

	fullWrite(static_cast<unsigned int>(std::stoul(commandToken[TOKEN_FULLWRITE_DATA], nullptr, 16)));

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleErase(const vector<string> commandToken)
{
	if (!isEraseCommandValid(commandToken))
		return COMMAND_INVALID_PARAM;

	int lba = stoi(commandToken[TOKEN_ERASE_LBA]);
	int size = stoi(commandToken[TOKEN_ERASE_SIZE]);

	std::ostringstream oss;

	if (size > 0) {

		int currentLba = lba;

		while (size > 0 && currentLba < MAX_LBA_SIZE) {
			int chunk = std::min(MAX_ERASE_LBA, size);
			chunk = std::min(chunk, MAX_LBA_SIZE - currentLba);

			erase(currentLba, chunk);

			currentLba += chunk;
			size -= chunk;
		}
	}
	else {
		int absSize = -size;
		int currentLba = lba;

		while (absSize > 0 && currentLba >= 0) {
			int chunk = std::min({ MAX_ERASE_LBA, absSize, currentLba + 1 });
			erase(currentLba - chunk + 1, chunk);

			currentLba -= chunk;
			absSize -= chunk;
		}
	}

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleEraseRange(const vector<string> commandToken)
{
	if (!isEraseRangeCommandValid(commandToken))
		return COMMAND_INVALID_PARAM;

	int startLba = stoi(commandToken[TOKEN_ERASE_RANGE_START_LBA]);
	int endLba = stoi(commandToken[TOKEN_ERASE_RANGE_END_LBA]);

	if (startLba > endLba) {
		ADD_LOG("ITestShell::handleEraseRange", "ERROR : startLba > endlba");
		return COMMAND_INVALID_PARAM;
	}
	ADD_LOG("ITestShell::handleEraseRange", "start LBA " + commandToken[TOKEN_ERASE_RANGE_START_LBA] + " end LBA " + commandToken[TOKEN_ERASE_RANGE_END_LBA]);

	int size = endLba - startLba + 1;
	int currentLba = startLba;

	while (size > 0 && currentLba < MAX_LBA_SIZE) {
		int chunk = std::min(MAX_ERASE_LBA, size);
		chunk = std::min(chunk, MAX_LBA_SIZE - currentLba);

		erase(currentLba, chunk);

		currentLba += chunk;
		size -= chunk;
	}

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleFlush(const vector<string> commandToken)
{
	if (!isFlushCommandValid(commandToken))
		return COMMAND_INVALID_PARAM;
	flush();
	return COMMAND_SUCCESS;
}

void ITestShell::setScript(const string& command, ITestScript* script)
{
	testScriptCommand[command] = script;
}

void ITestShell::fullWrite(const uint32_t data)
{
	ADD_LOG("ITestShell::fullWrite", "write all LBAs");
	for (int i = 0; i < MAX_LBA_SIZE; i++) {
		write(i, data);
	}
}

uint32_t ITestShell::fullRead()
{
	uint32_t readData = 0;

	ADD_LOG("ITestShell::fullRead", "read all LBAs");
	for (int lba = 0; lba < MAX_LBA_SIZE; lba++) {
		readData = read(lba);
	}

	return readData;
}

COMMAND_RESULT ITestShell::exit(const vector<string> commandToken) {
	ADD_LOG("ITestShell::exit", "exit");
	return COMMAND_EXIT;
}

COMMAND_RESULT ITestShell::help(const vector<string> commandToken)
{
	using std::left;
	using std::setw;
	using std::setfill;

	ADD_LOG("ITestShell::help", "print HELP message");

	cout << "ÆÀ¸í: A class\n";
	cout << "ÆÀ¿ø: ÃÖÀç¹Î, ÃÖÀ¯Á¤, ¼Òº´¿í, ±èÈñÁ¤, ±èÃæÈñ\n";

	cout << "\n=============================== command ===============================\n";
	cout << left << setfill(' ') << setw(35) << "write [LBA] [Value]" << "Write(data) at LBA(lba_num).ex: write 3 0xAAAABBBB\n";
	cout << left << setfill(' ') << setw(35) << "read [LBA]" << "Read data at LBA(lba_num).ex: read 3\n";
	cout << left << setfill(' ') << setw(35) << "exit" << "Exit from this program.ex: exit\n";
	cout << left << setfill(' ') << setw(35) << "help" << "Display help information.ex: help\n";
	cout << left << setfill(' ') << setw(35) << "fullwrite [Value]" << "Fill all LBA with(data).ex: fullwrite 0xAAAABBBB\n";
	cout << left << setfill(' ') << setw(35) << "fullread" << "Read all LBA data and display.ex: fullread\n";

	cout << "\n=============================== TC command ===============================\n";
	cout << left << setfill(' ') << setw(35) << "1_FullWriteAndReadCompare or 1_" << "Write all LBA and test all data is written with right data.\n";
	cout << left << setfill(' ') << setw(35) << "2_PartialLBAWrite or 2_" << "Write 5 LBAs and test all data is written with right data.Repeat 30 times.\n";
	cout << left << setfill(' ') << setw(35) << "3_WriteReadAging of 3_" << "Write LBA 0 and 99 and test all data is written with right data.Repeat 200 times.\n";
	cout << left << setfill(' ') << setw(35) << "4_EraseAndWriteAging or 4_" << "For each 3 LBAs, write and erase and test. Repeat 30 times.\n";

	return COMMAND_SUCCESS;
}
