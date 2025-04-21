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

CommandBuffer::~CommandBuffer()
{
	createTxtFilesOnDestruction();
}

void CommandBuffer::addCommandToBuffer(CommandValue command)
{
	if (buffer.size() >= maxBufferSize) {
		flush();
		buffer.push_back(command);
		return;
	}
	std::vector<int> checkBuffer = initCheckBufferWith(command);

	removeOverlappingWriteCommands(checkBuffer);

	if (command.command == CommandValue::ERASE) {
		setCheckBufferOnEraseCommands(checkBuffer);
		mergeEraseRange(checkBuffer);
	}
	else if (command.command == CommandValue::WRITE) {
		buffer.push_back(command);
	}

	removeOverwrittenSingleErase();
}

void CommandBuffer::flush()
{
	flushCommandsToSSD();
	renameBufferFilesToEmpty();

	buffer.clear();
}

bool CommandBuffer::getBufferedValueIfExists(int lba, uint32_t& outValue) const
{
	for (auto buf = buffer.rbegin(); buf != buffer.rend(); ++buf) {
		if (buf->command == CommandValue::WRITE && buf->LBA == lba) {
			outValue = buf->value;
			return true;
		}
		else if (buf->command == CommandValue::ERASE && lba >= buf->LBA && lba < buf->LBA + static_cast<int>(buf->value)) {
			outValue = CommandValue::EMPTY_VALUE;
			return true;
		}
	}
	return false;
}

std::string CommandBuffer::printBuffer() const
{
	std::string result = "";
	for (const auto& command : buffer) {
		result += command.getCommandStr() + "\n";
	}
	return result;
}

void CommandBuffer::removeOverwrittenSingleErase()
{
	std::vector<int> checkBuffer;
	checkBuffer.resize(CommandValue::MAX_NUM_LBA, CommandValue::NULL_COMMAND);

	for (auto buf = buffer.rbegin(); buf != buffer.rend(); ) {
		if (buf->command == CommandValue::WRITE) {
			checkBuffer[buf->LBA] = CommandValue::WRITE;
		}
		else if (buf->command == CommandValue::ERASE && buf->value == 1) {
			if (checkBuffer[buf->LBA] == CommandValue::WRITE) {
				buf = std::vector<CommandValue>::reverse_iterator(buffer.erase((buf + 1).base()));
				continue;
			}
		}
		buf++;
	}
}

void CommandBuffer::mergeEraseRange(std::vector<int>& checkBuffer)
{
	int minLBA = -1;
	for (int i = 0; i < CommandValue::MAX_NUM_LBA; i++) {
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
	if (minLBA != -1) {
		CommandValue mergedCommand(CommandValue::ERASE, minLBA, CommandValue::MAX_NUM_LBA - minLBA);
		buffer.insert(buffer.begin(), mergedCommand);
	}
}

void CommandBuffer::setCheckBufferOnEraseCommands(std::vector<int>& checkBuffer)
{
	for (auto buf = buffer.begin(); buf != buffer.end(); ) {
		if (buf->command == CommandValue::ERASE) {
			for (auto i = buf->LBA; i < buf->LBA + buf->value; i++) {
				checkBuffer[i] = CommandValue::ERASE;
			}
			buf = buffer.erase(buf);
			continue;
		}
		buf++;
	}
}

void CommandBuffer::removeOverlappingWriteCommands(std::vector<int>& checkBuffer)
{
	for (auto buf = buffer.begin(); buf != buffer.end(); ) {
		if (buf->command == CommandValue::WRITE) {
			if (checkBuffer[buf->LBA] != CommandValue::NULL_COMMAND) {
				buf = buffer.erase(buf);
				continue;
			}
		}
		buf++;
	}
}

std::vector<int> CommandBuffer::initCheckBufferWith(CommandValue& command)
{
	std::vector<int> checkBuffer;
	checkBuffer.resize(CommandValue::MAX_NUM_LBA, CommandValue::NULL_COMMAND);

	if (command.command == CommandValue::WRITE)
	{
		checkBuffer[command.LBA] = CommandValue::WRITE;
	}
	else if (command.command == CommandValue::ERASE)
	{
		for (auto i = command.LBA; i < command.LBA + command.value; i++) {
			checkBuffer[i] = CommandValue::ERASE;
		}
	}
	return checkBuffer;
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

void CommandBuffer::renameBufferFilesToEmpty()
{
	for (size_t i = 0; i < buffer.size(); ++i) {
		std::string oldPath = bufferDir + "/" + buffer[i].getCommandStr() + ".txt";
		std::string newName = "empty_" + std::to_string(i);
		std::string newPath = bufferDir + "/" + newName + ".txt";

		if (fs::exists(oldPath) && buffer[i].getCommandStr() != newName) {
			fs::rename(oldPath, newPath);
		}
	}
}

void CommandBuffer::flushCommandsToSSD()
{
	for (const auto& command : buffer) {
		if (command.command == CommandValue::WRITE) {
			ssd->writeLBA(command.LBA, command.value);
		}
		else if (command.command == CommandValue::ERASE) {
			for (int i = 0; i < static_cast<int>(command.value); ++i) {
				if (command.LBA + i >= 0 && command.LBA + i < CommandValue::MAX_NUM_LBA) {
					ssd->writeLBA(command.LBA + i, CommandValue::EMPTY_VALUE);
				}
			}
		}
	}
}
