#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <exception>
#include <iomanip>
#include "FileTextIO.h"

class CommandValue {
public:
	static const int WRITE = 1;
	static const int READ = 2;
	static const int ERASE = 3;

	int command;
	int LBA;
	uint32_t value;

	CommandValue(const std::string& input) {
		setCommand(input);
	}
	CommandValue(char chCommad, int LBA, uint32_t value) {
		if (chCommad == 'W') {
			command = WRITE;
		}
		else if (chCommad == 'E') {
			command = ERASE;
		}
		else if (chCommad == 'R') {
			command = READ;
		}
		else {
			command = 0;
		}
		this->LBA = LBA;
		this->value = value;
	}
	std::string getCommandStr() const
	{
		std::ostringstream oss;

		switch (command) {
		case WRITE: oss << "W"; break;
		case READ:  oss << "R"; break;
		case ERASE: oss << "E"; break;
		default:    oss << "?"; break;
		}

		oss << " " << LBA;

		if (command == WRITE)
		{
			oss << " 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << value;
		}
		else if (command == ERASE)
		{
			oss << " " << static_cast<int>(value);
		}

		return oss.str();

	}
	void setCommand(const std::string& input) {
		std::istringstream iss(input);
		std::string valueStr;
		char chCommad;

		if (!(iss >> chCommad >> LBA >> valueStr)) {
			return;
		}
		if (chCommad == 'W') {
			command = WRITE;
			value = std::stoul(valueStr, nullptr, 16);
		}
		else if (chCommad == 'E') {
			command = ERASE;
			value = std::stoi(valueStr);
		}
		else if (chCommad == 'R') {
			command = READ;
			value = 0;
		}
	}
};

class CommandBuffer {
private:
	std::vector<CommandValue> buffer;
	const int maxBufferSize = 5;
	const std::string bufferDir = "./buffer";

	void loadInitialFiles();
	void createTxtFilesOnDestruction() const;

public:
	CommandBuffer();
	~CommandBuffer();

	void addCommandToBuffer(CommandValue command);
	void flush();
	void printBuffer() const;
};