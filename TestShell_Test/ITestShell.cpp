#include <iostream>
#include <iomanip>
#include "ITestShell.h"

vector<string> ITestShell::splitBySpace(const string& input) {
	std::vector<std::string> tokens;
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

bool ITestShell::isWriteDataValid(const string& commandLine)
{
	vector<string> commandToken = splitBySpace(commandLine);
	// check the data input starts with "0x"
	if (commandToken[2].substr(0, 2) != "0x") {
		return false;
	}

	// check the data range after "0x". 
	for (size_t i = 2; i < commandToken[2].length(); ++i) {
		char c = commandToken[2][i];
		// should be one of those "A~F", "0~9"
		if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
			return false;
		}
	}
	return true;
}

bool ITestShell::isWriteCommandValid(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (commandToken.size() != 3) {
		return false;
	}
	if (!isValidLBA(commandToken)) {
		return false;
	}
	if (commandToken[2].length() != 10) {
		return false;
	}

	if (!isWriteDataValid(commandLine)) {
		return false;
	}

	return true;
}

bool ITestShell::isReadCommandValid(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (commandToken.size() != 2) {
		return false;
	}

	if (!isValidLBA(commandToken)) {
		return false;
	}

	return true;
}

bool ITestShell::isValidLBA(const vector<string>& commandToken)
{
	if (std::stoi(commandToken[1]) >= 100 || std::stoi(commandToken[1]) < 0) {
		return false;
	}

	return true;
}

bool ITestShell::isCommandValid(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (commandToken[0] == "read") {
		return isReadCommandValid(commandLine);
	}
	else if (commandToken[0] == "write") {
		return (isWriteCommandValid(commandLine));
	}
	else {
		return true;
	}
}

COMMAND_RESULT ITestShell::handleCommand(const string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (!isCommandValid(commandLine)) {
		return COMMAND_INVALID_PARAM; 
	}
	if (commandToken[0] == "read") {
		read(std::stoi(commandToken[1]));
	}
	else if (commandToken[0] == "write") {
		write(std::stoi(commandToken[1]), static_cast<unsigned int>(std::stoul(commandToken[2], nullptr, 16)));
	}
	else if (commandToken[0] == "fullread") {
		fullRead();
	}
	else if (commandToken[0] == "fullwrite") {
		fullWrite(static_cast<unsigned int>(std::stoul(commandToken[1], nullptr, 16)));
	}
	else if (commandToken[0] == "1_FullWriteAndReadCompare" || commandToken[0] == "1_") {
		fullWriteAndReadCompare();
	}
	else if (commandToken[0] == "2_PartialLBAWrite" || commandToken[0] == "2_") {
		partialLBAWrite();
	}
	else if (commandToken[0] == "3_WriteReadAging" || commandToken[0] == "3_") {
		writeReadAging();
	}
	else if (commandToken[0] == "help") {
		help();
	}
	else if (commandToken[0] == "exit") {
		return exit();
	}
	else {
		return COMMAND_INVALID_PARAM;
	}
	return COMMAND_SUCCESS;
}

void ITestShell::fullWrite(uint32_t data)
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
		std::cout << "LBA " << lba << " : expected : " << std::hex << expected << ", actual : " << data << std::endl;
		return false;
	}
}

bool ITestShell::fullWriteAndReadCompare() {
	uint32_t data = 0x2345bcde;
	bool result;

	for (int i = 0; i < 100; i += 5) {
		for (int j = 0; j < 5; j++) {
			write(i + j, data);
		}

		for (int j = 0; j < 5; j++) {
			result = readCompare(i + j, data);
			if (result == false) {
				std::cout << "FullWriteAndReadCompare FAIL\n";
				return false;
			}
		}
	}
	std::cout << "FullWriteAndReadCompare PASS\n";
	return true;
}

bool ITestShell::partialLBAWrite() {
	uint32_t data = 0x2345bcde;
	bool result;
	int seq[5] = {4, 0, 3, 1, 2};

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 5; j++) {
			write(seq[j], data);
		}
		for (int j = 0; j < 5; j++) {
			result = readCompare(j, data);
			if (result == false) {
				std::cout << "PartialLBAWrite FAIL\n";
				return false;
			}
		}
	}

	std::cout << "PartialLBAWrite PASS\n";
	return true;
}

bool ITestShell::writeReadAging() {
	uint32_t data = 0x2345bcde;
	bool result;

	for (int i = 0; i < 200; i++) {
		write(0, data);
		write(99, data);

		result = readCompare(0, data);
		if (result == false) {
			std::cout << "WriteReadAging FAIL\n";
			return false;
		}
		result = readCompare(99, data);
		if (result == false) {
			std::cout << "WriteReadAging FAIL\n";
			return false;
		}
	}
	std::cout << "WriteReadAging PASS\n";
	return true;
}

void ITestShell::help()
{
	std::cout << "����: A class\n";
	std::cout << "����: �����, ������, �Һ���, ������, ������\n";
	std::cout << "============ command information (format: command : description) ============\n";
	std::cout << std::left << std::setw(40) << "write (lba_num) (data)" << "Write(data) at LBA(lba_num).ex: write 3 0xAAAABBBB\n";
	std::cout << std::left << std::setw(40) << "read (lba_num)" << "Read data at LBA(lba_num).ex: read 3\n";
	std::cout << std::left << std::setw(40) << "exit" << "Exit from this program.ex: exit\n";
	std::cout << std::left << std::setw(40) << "help" << "Display help information.ex: help\n";
	std::cout << std::left << std::setw(40) << "fullwrite (data)" << "Fill all LBA with(data).ex: fullwrite 0xAAAABBBB\n";
	std::cout << std::left << std::setw(40) << "fullread" << "Read all LBA data and display.ex: fullread\n";

	std::cout << "============ TC information ============\n";
	std::cout << std::left << std::setw(40)  << "1_FullWriteAndReadCompare"	<< "Write all LBA and test all data is written with right data.\n";
	std::cout << "  ex: 1_FullWriteAndReadCompare or 1_\n";
	std::cout << std::left << std::setw(40) << "2_PartialLBAWrite" << "Write 5 LBAs and test all data is written with right data.Repeat 30 times.\n";
	std::cout << "  ex: 2_PartialLBAWrite or 2_\n";
	std::cout << std::left << std::setw(40) << "3_WriteReadAging" << "Write LBA 0 and 99 and test all data is written with right data.Repeat 200 times.\n";
	std::cout << "  ex: 3_WriteReadAging or 3_\n";
}
