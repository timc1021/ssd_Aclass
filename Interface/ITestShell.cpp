#include <iostream>
#include <iomanip>
#include <sstream>
#include "ITestShell.h"
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
	TOKEN_ERASE_RANGE_START_LBA = 1,
	TOKEN_ERASE_RANGE_END_LBA,
	TOKEN_ERASE_RANGNE_NUM,
} ERASE_RANGE_TOKEN;

COMMAND_RESULT ITestShell::handleCommand(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);
	string command = commandToken[TOKEN_COMMAND];
	COMMAND_RESULT result = COMMAND_SUCCESS;

	if (!isCommandValid(commandToken)) {
		return COMMAND_INVALID_PARAM;
	}

	if (command == "read") {
		result = handleRead(commandToken);
	}
	else if (command == "write") {
		result = handleWrite(commandToken);
	}
	else if (command == "fullread") {
		result = handleFullread();
	}
	else if (command == "fullwrite") {
		result = handleFullwrite(commandToken);
	}
	else if (command == "erase") {
		result = handleErase(commandToken);
	}
	else if (command == "erase_range") {
		result = handleEraseRange(commandToken);
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
	else if (command == "4_EraseAndWriteAging" || command == "4_") {
		eraseAndWriteAging();
	}
	else if (command == "help") {
		help();
	}
	else if (command == "exit") {
		result = exit();
	}

	return result;
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

bool ITestShell::isCommandValid(const vector<string> commandToken) {
	string command = commandToken[TOKEN_COMMAND];
	bool exists = find(commandList.begin(), commandList.end(), command) != commandList.end();

	if (!exists) {
		Logger::getInstance().addLog("ITestShell::isCommandValid", "ERROR");
		return false;
	}

	return true;
}

bool ITestShell::isWriteDataValid(const string& writeData)
{
	if ((writeData.length() != DATA_LENGTH) ||
		(writeData.substr(0, HEX_PREFIX_LENGTH) != HEX_PREFIX)) {
		Logger::getInstance().addLog("ITestShell::isWriteDataValid", "ERROR");

		return false;
	}

	for (char c : writeData.substr(HEX_PREFIX_LENGTH)) {
		if (isdigit(c) || isupper(c)) continue;
		
		Logger::getInstance().addLog("ITestShell::isWriteDataValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isWriteCommandValid(const vector<string> commandToken) {
	
	if ((commandToken.size() != TOKEN_WRITE_NUM) ||
		(!isLBAValid(commandToken[TOKEN_WRITE_LBA])) ||
		(!isWriteDataValid(commandToken[TOKEN_WRITE_DATA]))) {
		Logger::getInstance().addLog("ITestShell::isReadCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isReadCommandValid(const vector<string> commandToken) {
	if ((commandToken.size() != TOKEN_READ_NUM) ||
		(!isLBAValid(commandToken[TOKEN_READ_LBA]))) {
		Logger::getInstance().addLog("ITestShell::isReadCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isFullwriteCommandValid(const vector<string> commandToken)
{
	if ((commandToken.size() != TOKEN_FULLWRITE_NUM) ||
		(!isWriteDataValid(commandToken[TOKEN_FULLWRITE_DATA]))) {
		Logger::getInstance().addLog("ITestShell::isFullwriteCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isLBAValid(const string& lba)
{
	int iLba = std::stoi(lba);

	if (iLba >= MAX_LBA_SIZE || iLba < START_LBA) {
		Logger::getInstance().addLog("ITestShell::isLBAValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isEraseCommandValid(const vector<string> commandToken)
{
	if ((commandToken.size() != TOKEN_ERASE_NUM) ||
		(!isLBAValid(commandToken[TOKEN_ERASE_LBA]))) {
		Logger::getInstance().addLog("ITestShell::isEraseCommandValid", "ERROR");

		return false;
	}

	return true;
}

bool ITestShell::isEraseRangeCommandValid(const vector<string> commandToken)
{
	if ((commandToken.size() != TOKEN_ERASE_RANGNE_NUM) ||
		(!isLBAValid(commandToken[TOKEN_ERASE_RANGE_START_LBA])) ||
		(!isLBAValid(commandToken[TOKEN_ERASE_RANGE_END_LBA]))) {
		Logger::getInstance().addLog("ITestShell::isEraseRangeCommandValid", "ERROR");

		return false;
	}

	return true;
}

COMMAND_RESULT ITestShell::handleRead(const vector<string> commandToken)
{
	if (!isReadCommandValid(commandToken))
		return COMMAND_INVALID_PARAM;

	read(stoi(commandToken[TOKEN_READ_LBA]));

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleFullread(void)
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

			oss << "SUCCESS : erase(" << currentLba << ", " << chunk << ")\n";
			Logger::getInstance().addLog("ITestShell::handleErase", oss.str());

			erase(currentLba, chunk);

			oss << "erase(" << lba << ", " << chunk << ")";
			Logger::getInstance().addLog("ITestShell::executeErase", oss.str());

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
			
			std::ostringstream oss;
			oss << "erase(" << lba << ", " << chunk << ")\n" ;
			Logger::getInstance().addLog("ITestShell::executeErase", oss.str());

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
		Logger::getInstance().addLog("ITestShell::handleEraseRange", "ERROR : startLba > endlba");
		return COMMAND_INVALID_PARAM;
	}

	int size = endLba - startLba + 1;
	int currentLba = startLba;

	while (size > 0 && currentLba < MAX_LBA_SIZE) {
		int chunk = std::min(MAX_ERASE_LBA, size);
		chunk = std::min(chunk, MAX_LBA_SIZE - currentLba);

		int rangeStart = currentLba;
		int rangeEnd = currentLba + chunk - 1;

		std::ostringstream oss;
		oss << "SUCCESS : erase(" << rangeStart << ", " << rangeEnd << ")";
		Logger::getInstance().addLog("ITestShell::handleEraseRange", oss.str());

		eraseRange(rangeStart, rangeEnd);

		currentLba += chunk;
		size -= chunk;
	}

	return COMMAND_SUCCESS;
}

void ITestShell::fullWrite(const uint32_t data)
{
	for (int i = 0; i < MAX_LBA_SIZE; i++) {
		write(i, data);
	}
}

uint32_t ITestShell::fullRead()
{
	uint32_t readData = 0;

	for (int lba = 0; lba < MAX_LBA_SIZE; lba++) {
		readData = read(lba);
	}

	return readData;
}

COMMAND_RESULT ITestShell::exit() {
	return COMMAND_EXIT;
}

bool ITestShell::readCompare(int lba, const uint32_t expected) {
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

	for (int lba_base = 0; lba_base < MAX_LBA_SIZE; lba_base += 5) {
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
	int seq[5] = { 4, 0, 3, 1, 2 };

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

bool ITestShell::eraseAndWriteAging() {
	uint32_t write_data = getRandUint32();
	uint32_t overwrite_data = getRandUint32();
	bool result = false;

	eraseRange(0, 2);

	for (int loop = 0; loop < 30; loop++) {
		for (int lba_base = 2; lba_base + 2 < MAX_LBA_SIZE; lba_base += 2) {
			write(lba_base, write_data);
			write(lba_base, overwrite_data);
			eraseRange(lba_base, lba_base + 2);

			for (int lba_offset = 0; lba_offset < 3; lba_offset++) {
				if (readCompare(lba_base + lba_offset, 0) == false) {
					cout << "EraseAndWriteAging FAIL\n";
					return false;
				}
			}
		}
	}

	cout << "EraseAndWriteAging PASS\n";
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
	cout << left << setfill(' ') << setw(35) << "1_FullWriteAndReadCompare or 1_" << "Write all LBA and test all data is written with right data.\n";
	cout << left << setfill(' ') << setw(35) << "2_PartialLBAWrite or 2_" << "Write 5 LBAs and test all data is written with right data.Repeat 30 times.\n";
	cout << left << setfill(' ') << setw(35) << "3_WriteReadAging of 3_" << "Write LBA 0 and 99 and test all data is written with right data.Repeat 200 times.\n";
	cout << left << setfill(' ') << setw(35) << "4_EraseAndWriteAging or 4_" << "For each 3 LBAs, write and erase and test. Repeat 30 times.\n";
}
