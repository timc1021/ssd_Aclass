#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <exception>
#include <iomanip>
#include "FileTextIO.h"
#include "SSDControllerInterface.h"

class CommandValue {
public:
	static const int NULL_COMMAND = 0;
	static const int WRITE = 1;
	static const int READ = 2;
	static const int ERASE = 3;

	int command;

	static const int MAX_NUM_LBA = 100;

	int LBA;
	uint32_t value;

	CommandValue(const std::string& input) {
		setCommand(input);
	}
	CommandValue(char chCommand, int LBA, uint32_t value) {
		if (chCommand == 'W') {
			command = WRITE;
		}
		else if (chCommand == 'E') {
			command = ERASE;
		}
		else if (chCommand == 'R') {
			command = READ;
		}
		else {
			command = 0;
		}
		this->LBA = LBA;
		this->value = value;
	}
	CommandValue(int command, int LBA, uint32_t value) {
		this->command = command;
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
	std::shared_ptr<SSDControllerInterface> ssd;

	void loadInitialFiles();
	void createTxtFilesOnDestruction() const;

	void mergeEraseRange(std::vector<int>& checkBuffer);
	void removeOverlappingEraseCommands(CommandValue& command, std::vector<int>& checkBuffer);
	void removeOverlappingWriteCommands(std::vector<int>& checkBuffer);
	std::vector<int> initCheckBufferWith(CommandValue& command);

	void renameBufferFilesToEmpty();
	void flushCommandsToSSD();

public:
	CommandBuffer(std::shared_ptr<SSDControllerInterface> ssd);
	~CommandBuffer();

	void addCommandToBuffer(CommandValue command);

	void flush();



	std::string printBuffer() const;
	bool getBufferedValueIfExists(int lba, uint32_t& outValue) const;
};