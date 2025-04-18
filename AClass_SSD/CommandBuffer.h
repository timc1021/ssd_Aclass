#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <exception>
#include "FileTextIO.h"

class CommandValue {
public:
	static const int WRITE = 1;
	static const int READ = 2;
	static const int ERASE = 3;

	std::string strFUllCommand;
	int command;
	int LBA;
	uint32_t value;

	CommandValue(const std::string& input) {
		setCommand(input);
	}
	CommandValue(char command, int LBA, uint32_t value) : {
		// TO-Do
	}
	void setCommand(const std::string& input) {
		std::istringstream iss(input);
		std::string valueStr;
		char chCommad;
		strFUllCommand = input;

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