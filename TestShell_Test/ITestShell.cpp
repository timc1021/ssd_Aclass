#include <iostream>
#include <iomanip>
#include "ITestShell.h"

using std::cout;
using std::isdigit;
using std::stoi;

#define TOKEN_COMMAND (0)

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

#define HEX_PREFIX			("0x")
#define HEX_PREFIX_LENGTH	(2)

bool ITestShell::isWriteDataValid(const string& writeData)
{
	if (writeData.length() != 10) {
		return false;
	}

	if (writeData.substr(0, HEX_PREFIX_LENGTH) != HEX_PREFIX) {
		return false;
	}

	for (char c : writeData.substr(HEX_PREFIX_LENGTH)) {
		if (isdigit(c) || isupper(c)) continue;
		return false;
	}

	return true;
}

bool ITestShell::isWriteCommandValid(const vector<string> commandToken) {
		if (commandToken.size() != TOKEN_WRITE_NUM) {
		return false;
	}
	if (!isValidLBA(commandToken[TOKEN_WRITE_LBA])) {
		return false;
	}

	if (!isWriteDataValid(commandToken[TOKEN_WRITE_DATA])) {
		return false;
	}

	return true;
}

bool ITestShell::isReadCommandValid(const vector<string> commandToken) {

	if (commandToken.size() != TOKEN_READ_NUM) {
		return false;
	}

	if (!isValidLBA(commandToken[TOKEN_READ_LBA])) {
		return false;
	}

	return true;
}

bool ITestShell::isFullwriteCommandValid(const vector<string> commandToken)
{
	if (commandToken.size() != TOKEN_FULLWRITE_NUM) {
		return false;
	}

	if (!isWriteDataValid(commandToken[TOKEN_FULLWRITE_DATA])) {
		return false;
	}

	return true;
}

bool ITestShell::isValidLBA(const string& lba)
{
	int iLba = std::stoi(lba);

	if (iLba >= MAX_LBA_SIZE || iLba < START_LBA) {
		return false;
	}

	return true;
}

bool ITestShell::isCommandValid(const vector<string> commandToken) {
	string command = commandToken[TOKEN_COMMAND];

	bool exists = find(commandList.begin(), commandList.end(), command) != commandList.end();
	if (!exists) return false;
	
	return true;
}

COMMAND_RESULT ITestShell::handleCommand(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);
	string command = commandToken[TOKEN_COMMAND];

	if (!isCommandValid(commandToken)) {
		return COMMAND_INVALID_PARAM; 
	}

	if (command == "read") {
		if (!isReadCommandValid(commandToken))
			return COMMAND_INVALID_PARAM;

		read(stoi(commandToken[TOKEN_READ_LBA]));
	}
	else if (command == "write") {
		if (!isWriteCommandValid(commandToken))
			return COMMAND_INVALID_PARAM;

		write(stoi(commandToken[TOKEN_WRITE_LBA]), static_cast<unsigned int>(std::stoul(commandToken[TOKEN_WRITE_DATA], nullptr, 16)));
	}
	else if (command == "fullread") {
		fullRead();
	}
	else if (command == "fullwrite") {
		if (!isFullwriteCommandValid(commandToken))
			return COMMAND_INVALID_PARAM;

		fullWrite(static_cast<unsigned int>(std::stoul(commandToken[TOKEN_FULLWRITE_DATA], nullptr, 16)));
	}
	else if (command == "erase") {
		erase(1, 5); // TODO
	}
	else if (command == "erase_range") {
		eraseRange(1, 6); // TODO
	}
	else if (command == "1_FullWriteAndReadCompare" || command == "1_") {
		fullWriteAndReadCompare();
	}
	else if (command == "2_PartialLBAWrite" || command == "2_") {
		partialLBAWrite();
	}
	else if (command == "3_WriteReadAging" || command == "3_") {
		writeReadAging();
	}
	else if (command == "help") {
		help();
	}
	else if (command == "exit") {
		return exit();
	}

	return COMMAND_SUCCESS;
}

void ITestShell::fullWrite(const uint32_t data)
{
	for (int i = 0; i < MAX_LBA_SIZE; i++) {
		write(i, data);
	}
}

COMMAND_RESULT ITestShell::exit() {
	return COMMAND_EXIT;
}

uint32_t ITestShell::fullRead()
{
	uint32_t readData = 0;

	for (int lba = 0; lba < MAX_LBA_SIZE; lba++) {
		readData = read(lba);
	}

	return readData;
}

bool ITestShell::readCompare(int lba, uint32_t expected) {
	int data = read(lba);

	if (data == expected)
		return true;
	else {
		std::cout << "LBA " << lba << " : expected : 0x" << std::hex << expected << ", actual : 0x" << data << std::endl;
		return false;
	}
}

uint32_t getRandUint32()
{
	return (rand() | (rand() << 15) | ((rand() & 0x3) << 30));
}

bool ITestShell::fullWriteAndReadCompare() {
	uint32_t data;
	bool result;

	for (int lba_base = 0; lba_base < ITestShell::MAX_LBA_SIZE; lba_base += 5) {
		data = getRandUint32();

		for (int lba_offset = 0; lba_offset < 5; lba_offset++) {
			write(lba_base + lba_offset, data);
		}

		for (int lba_offset = 0; lba_offset < 5; lba_offset++) {
			result = readCompare(lba_base + lba_offset, data);
			if (result == false) {
				cout << "FullWriteAndReadCompare FAIL\n";
				return false;
			}
		}
	}
	cout << "FullWriteAndReadCompare PASS\n";
	return true;
}

bool ITestShell::partialLBAWrite() {
	uint32_t data;
	bool result;
	int seq[5] = {4, 0, 3, 1, 2};

	for (int loop = 0; loop < 30; loop++) {
		data = getRandUint32();

		for (int seq_idx = 0; seq_idx < 5; seq_idx++) {
			write(seq[seq_idx], data);
		}

		for (int lba = 0; lba < 5; lba++) {
			result = readCompare(lba, data);
			if (result == false) {
				cout << "PartialLBAWrite FAIL\n";
				return result;
			}
		}
	}

	cout << "PartialLBAWrite PASS\n";
	return true;
}

bool ITestShell::writeReadAging() {
	uint32_t data = 0;
	bool result = false;

	for (int loop = 0; loop < 200; loop++) {
		data = getRandUint32();

		write(0, data);
		write(99, data);

		if (readCompare(0, data) == false || readCompare(99, data) == false) {
			cout << "WriteReadAging FAIL\n";
			return false;
		}
		result = readCompare(99, data);
		if (result == false) {
			cout << "WriteReadAging FAIL\n";
			return false;
		}
	}
	cout << "WriteReadAging PASS\n";
	return true;
}

void ITestShell::help()
{
	using std::left;
	using std::setw;
	using std::setfill;

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
	cout << left << setfill(' ') << setw(35)  << "1_FullWriteAndReadCompare or 1_"	<< "Write all LBA and test all data is written with right data.\n";
	cout << left << setfill(' ') << setw(35) << "2_PartialLBAWrite or 2_" << "Write 5 LBAs and test all data is written with right data.Repeat 30 times.\n";
	cout << left << setfill(' ') << setw(35) << "3_WriteReadAging of 3_" << "Write LBA 0 and 99 and test all data is written with right data.Repeat 200 times.\n";
}
