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
	bool merge_flag = false;

	if (buffer.size() >= maxBufferSize) {
		flush();
		buffer.push_back(command);
		return;
	}

	if (command.command == CommandValue::WRITE) {
		for (auto i = 0; i < buffer.size(); i++) {
			if (buffer[i].LBA == command.LBA && buffer[i].command == CommandValue::WRITE) {
				buffer.erase(buffer.begin() + i);
				i--;
				continue;
			}
		}
	}
	else if (command.command == CommandValue::ERASE) {
		for (auto i = 0; i < buffer.size(); i++) {
			if (buffer[i].command == CommandValue::WRITE) {
				for (auto j = command.LBA; j < command.LBA + command.value; j++) {
					if (j == buffer[i].LBA) {
						buffer.erase(buffer.begin() + i);
						i--;
						break;
					}
				}
			}
			else if (buffer[i].command == CommandValue::ERASE) {
				if (command.LBA <= buffer[i].LBA && buffer[i].LBA + buffer[i].value - 1 <= command.LBA + command.value - 1)
				{
					buffer.erase(buffer.begin() + i);
					i--;
				}
				else if (command.LBA >= buffer[i].LBA && buffer[i].LBA + buffer[i].value - 1 >= command.LBA + command.value - 1)
				{
					merge_flag = true;
				}
				else if (buffer[i].LBA >= command.LBA && buffer[i].LBA - 1 <= command.LBA + command.value - 1)
				{
					buffer[i].value += buffer[i].LBA - command.LBA;
					buffer[i].LBA = command.LBA;
					merge_flag = true;
				}
				else if (buffer[i].LBA + buffer[i].value >= command.LBA && buffer[i].LBA + buffer[i].value - 1 <= command.LBA + command.value - 1)
				{
					buffer[i].value += command.LBA + command.value - 1 - (buffer[i].LBA + buffer[i].value - 1);
					merge_flag = true;
				}
			}
		}
	}
	if (merge_flag == false)
		buffer.push_back(command);
}


void CommandBuffer::printBuffer() const
{
	std::cout << "[Buffer Contents]\n";
	for (const auto& command : buffer) {
		std::cout << "- " << command.getCommandStr() << "\n";
	}
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