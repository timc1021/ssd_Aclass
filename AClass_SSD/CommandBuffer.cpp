#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "CommandBuffer.h"

namespace fs = std::filesystem;

CommandBuffer::CommandBuffer()
{
	loadInitialFiles();
}

CommandBuffer::~CommandBuffer() {
	createTxtFilesOnDestruction();
}

void CommandBuffer::addCommandToBuffer(CommandValue command)
{
	if (buffer.size() >= maxBufferSize) {
		flush();
		buffer.push_back(command);
		return;
	}

	buffer.push_back(command);
}

void CommandBuffer::printBuffer() const
{
	std::cout << "[Buffer Contents]\n";
	for (const auto& command : buffer) {
		std::cout << "- " << command.strFUllCommand << "\n";
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
		std::string path = bufferDir + "/" + std::to_string(i) + buffer[i].strFUllCommand + ".txt";
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
	//To-do: ssd nand file should be modified!!

	for (size_t i = 0; i < buffer.size(); ++i) {
		std::string oldPath = bufferDir + "/" + buffer[i].strFUllCommand + ".txt";
		std::string newName = "empty_" + std::to_string(i);
		std::string newPath = bufferDir + "/" + newName + ".txt";

		if (fs::exists(oldPath) && buffer[i].strFUllCommand != newName) {
			fs::rename(oldPath, newPath);
		}

		buffer[i] = newName;
	}

	buffer.clear();
}