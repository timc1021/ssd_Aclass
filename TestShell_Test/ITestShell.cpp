#include <iostream>
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

COMMAND_RESULT ITestShell::checkWriteCommandError(string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (commandToken.size() != 3)
		return COMMAND_INVALID_PARAM;
	if (std::stoi(commandToken[1]) >= 100 || std::stoi(commandToken[1]) < 0)
		return COMMAND_INVALID_PARAM;
	if (commandToken[2].length() != 10)
		return COMMAND_INVALID_PARAM;

	// check the data input starts with "0x"
	if (commandToken[2].substr(0, 2) != "0x") {
		return COMMAND_INVALID_PARAM;
	}

	// check the data range after "0x". 
	for (size_t i = 2; i < commandToken[2].length(); ++i) {
		char c = commandToken[2][i];
		// should be one of those "A~F", "0~9"
		if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) {
			return COMMAND_INVALID_PARAM;
		}
	}

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::checkReadCommandError(string& commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (commandToken.size() != 2)
		return COMMAND_INVALID_PARAM;
	if (std::stoi(commandToken[1]) >= 100 || std::stoi(commandToken[1]) < 0)
		return COMMAND_INVALID_PARAM;

	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::checkCommandError(string & commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (commandToken[0] == "read") {
		if (checkReadCommandError(commandLine) == COMMAND_INVALID_PARAM) {
			return COMMAND_INVALID_PARAM;
		}
	}
	else if (commandToken[0] == "write") {
		if (checkWriteCommandError(commandLine) == COMMAND_INVALID_PARAM) {
			return COMMAND_INVALID_PARAM;
		}
	}
	return COMMAND_SUCCESS;
}

COMMAND_RESULT ITestShell::handleCommand(string commandLine) {
	vector<string> commandToken = splitBySpace(commandLine);

	if (checkCommandError(commandLine) == COMMAND_INVALID_PARAM)
		return COMMAND_INVALID_PARAM;

	if (commandToken[0] == "read") {
		read(std::stoi(commandToken[1]));
	}
	else if (commandToken[0] == "write") {
		write(std::stoi(commandToken[1]), static_cast<unsigned int>(std::stoul(commandToken[2], nullptr, 16)));
	}
	else if (commandToken[0] == "fullRead") {
		fullRead();
	}
	else if (commandToken[0] == "fullWrite") {
		fullWrite(0xAAAABBBB); // TODO
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

void ITestShell::write(int lba, uint32_t data)
{
	// TODO : system("ssd.exe");
	std::cout << "write done, lba : " << lba << ", data : " << data << std::endl;
}

void ITestShell::fullWrite(uint32_t data)
{
	for (int i = 0; i < 100; i++) {
		write(i, data);
	}
}

COMMAND_RESULT ITestShell::exit() {
	return COMMAND_EXIT;
}

uint32_t ITestShell::read(int lba)
{
	int result = 0;
	uint32_t read_data = 0;
	// result = system("ssd.exe");

	if (result == 0) {
		std::cout << "read done, lba : " << lba << ", data : " << read_data << std::endl;
	}
	else {
		return -1;
	}
	return read_data;
}

uint32_t ITestShell::fullRead()
{
	int result = 0;
	uint32_t read_data = 0;

	for (int lba = 0; lba < 100; lba++) {
		read_data = read(lba);
	}

	if (result == 0) {
		std::cout << "full read done\n";
	}
	else {
		return -1;
	}

	return result;
}

void ITestShell::help()
{
	std::cout << "����: A class\n";
	std::cout << "����: �����, ������, �Һ���, ������, ������\n";
	std::cout << "============ command information (format: command : description) ============\n";
	std::cout << "write (lba_num) (data): Write (data) at LBA(lba_num). ex: write 3 0xAAAABBBB\n";
	std::cout << "read (lba_num): Read data at LBA(lba_num). ex: read 3\n";
	std::cout << "exit: Exit from this program. ex: exit\n";
	std::cout << "help: Display help information. ex: help\n";
	std::cout << "fullwrite (data): Fill all LBA with (data). ex: fullwrite 0xAAAABBBB\n";
	std::cout << "fullread: Read all LBA data and display. ex: fullread\n";

	std::cout << "1_FullWriteAndReadCompare: Write all LBA and test all data is written with right data. ex: 1_FullWriteAndReadCompare or 1_\n";
	std::cout << "2_PartialLBAWrite: Write 5 LBAs and test all data is written with right data. Repeat 30 times. ex: 2_PartialLBAWrite or 2_\n";
	std::cout << "3_WriteReadAging: Write LBA 0 and 99 and test all data is written with right data. Repeat 200 times. ex: 3_WriteReadAging or 3_\n";

	std::cout << "help done\n";
}
