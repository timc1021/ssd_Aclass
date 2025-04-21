#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "CommandBuffer.h"

namespace fs = std::filesystem;

CommandBuffer::CommandBuffer(std::shared_ptr<SSDControllerInterface> ssd) : ssd(std::move(ssd))
{
	loadInitialFiles();
}

CommandBuffer::~CommandBuffer() {
	createTxtFilesOnDestruction();
}

void CommandBuffer::addCommandToBuffer(CommandValue command)
{
	int cmdCheckBuffer[CommandValue::MAX_NUM_LBA] = { CommandValue::NULL_COMMAND, };
	int checkBuffer[CommandValue::MAX_NUM_LBA] = { CommandValue::NULL_COMMAND, };

	if (buffer.size() >= maxBufferSize) {
		flush();
		buffer.push_back(command);
		return;
	}
	if (command.command == CommandValue::WRITE)
	{
		checkBuffer[command.LBA] = CommandValue::WRITE;
		cmdCheckBuffer[command.LBA] = CommandValue::WRITE;
	}
	else if (command.command == CommandValue::ERASE)
	{
		for (auto i = command.LBA; i < command.LBA + command.value; i++) {
			checkBuffer[i] = CommandValue::ERASE;
			cmdCheckBuffer[i] = CommandValue::ERASE;
		}
	}


	for (auto buf = buffer.begin(); buf != buffer.end(); ) {
		if (buf->command == CommandValue::WRITE) {
			if (cmdCheckBuffer[buf->LBA] != CommandValue::NULL_COMMAND) {
				buf = buffer.erase(buf);
				continue;
			}
		}
		else if (buf->command == CommandValue::ERASE && command.command == CommandValue::ERASE) {
			for (auto i = buf->LBA; i < buf->LBA + buf->value; i++) {
				checkBuffer[i] = CommandValue::ERASE;
			}
			buf = buffer.erase(buf);
			continue;
		}
		buf++;
	}

	if (command.command == CommandValue::ERASE) {
		for (int i = 0, minLBA = -1; i < CommandValue::MAX_NUM_LBA; i++) {
			if (checkBuffer[i] == CommandValue::ERASE && minLBA == -1)
			{
				minLBA = i;
			}
			else if (minLBA != -1 && i - minLBA >= 10) {
				CommandValue mergedCommand(CommandValue::ERASE, minLBA, 10);
				minLBA += 10;

				if (checkBuffer[minLBA] != CommandValue::ERASE)
					minLBA = -1;

				if (buffer.size() >= maxBufferSize)
					flush();
				buffer.insert(buffer.begin(), mergedCommand);
			}
			else if (minLBA != -1 && checkBuffer[i] != CommandValue::ERASE)
			{
				CommandValue mergedCommand(CommandValue::ERASE, minLBA, i - minLBA);
				minLBA = -1;

				if (buffer.size() >= maxBufferSize)
					flush();
				buffer.insert(buffer.begin(), mergedCommand);
			}
		}
	}
	else if (command.command == CommandValue::WRITE) {
		buffer.push_back(command);
	}
}


std::string CommandBuffer::printBuffer() const
{
	std::string result = "";
	for (const auto& command : buffer) {
		result += command.getCommandStr() + "\n";
	}
	return result;
}

void CommandBuffer::loadInitialFiles() {
	if (!fs::exists(bufferDir)) {
		return;
	}

	for (const auto& entry : fs::directory_iterator(bufferDir)) {
		std::string filename = entry.path().filename().stem().string();
		if (!filename.empty() && filename.substr(1) != "empty") {
			CommandValue c(filename.substr(1));
			buffer.push_back(c);
		}
	}
}
void CommandBuffer::createTxtFilesOnDestruction() const {
	if (!fs::exists(bufferDir)) {
		fs::create_directory(bufferDir);
	}
	else {
		for (const auto& entry : fs::directory_iterator(bufferDir)) {
			if (entry.is_regular_file()) {
				fs::remove(entry.path());
			}
		}
	}

	for (int i = 0; i < buffer.size(); i++) {
		std::string path = bufferDir + "/" + std::to_string(i) + buffer[i].getCommandStr() + ".txt";
		std::ofstream ofs(path);
		ofs.close();
	}

	for (size_t i = buffer.size(); i < maxBufferSize; ++i) {
		std::string filename = std::to_string(i) + "empty.txt";
		std::string path = bufferDir + "/" + filename;
		std::ofstream ofs(path);
		ofs.close();
	}
}
void CommandBuffer::flush() {
	for (const auto& command : buffer) {
		if (command.command == CommandValue::WRITE) {
			ssd->writeLBA(command.LBA, command.value);
		}
		else if (command.command == CommandValue::ERASE) {
			for (int i = 0; i < static_cast<int>(command.value); ++i) {
				if (command.LBA + i >= 0 && command.LBA + i < 100) {
					ssd->writeLBA(command.LBA + i, 0x00000000);
				}
			}
		}
	}

	for (size_t i = 0; i < buffer.size(); ++i) {
		std::string oldPath = bufferDir + "/" + buffer[i].getCommandStr() + ".txt";
		std::string newName = "empty_" + std::to_string(i);
		std::string newPath = bufferDir + "/" + newName + ".txt";

		if (fs::exists(oldPath) && buffer[i].getCommandStr() != newName) {
			fs::rename(oldPath, newPath);
		}

		buffer[i] = CommandValue(newName);
	}

	buffer.clear();
}
bool CommandBuffer::getBufferedValueIfExists(int lba, uint32_t& outValue) const {
	for (auto it = buffer.rbegin(); it != buffer.rend(); ++it) {
		if (it->command == CommandValue::WRITE && it->LBA == lba) {
			outValue = it->value;
			return true;
		}
		else if (it->command == CommandValue::ERASE && lba >= it->LBA && lba < it->LBA + static_cast<int>(it->value)) {
			outValue = 0x00000000;
			return true;
		}
	}
	return false;
}