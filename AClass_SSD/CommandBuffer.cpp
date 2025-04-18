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

void CommandBuffer::addCommandToBuffer(std::string command)
{
	if (buffer.size() >= maxBufferSize) {
		flush();
	}
	buffer.push_back(command);
}

void CommandBuffer::printBuffer() const
{
	std::cout << "[Buffer Contents]\n";
	for (const auto& name : buffer) {
		std::cout << "- " << name << "\n";
	}
}

void CommandBuffer::loadInitialFiles() {
	if (!fs::exists(bufferDir)) {
		return;
	}

	for (const auto& entry : fs::directory_iterator(bufferDir)) {
		if (entry.is_regular_file()) {
			buffer.push_back(entry.path().filename().stem().string());
		}
	}
}

void CommandBuffer::createTxtFilesOnDestruction() const {
	if (!fs::exists(bufferDir)) {
		fs::create_directory(bufferDir);
	}

	for (const auto& name : buffer) {
		std::string path = bufferDir + "/" + name + ".txt";
		std::ofstream ofs(path);
		ofs.close();
	}
}
void CommandBuffer::flush() {
	//To-do: ssd nand file should be modified!!

	for (size_t i = 0; i < buffer.size(); ++i) {
		std::string oldPath = bufferDir + "/" + buffer[i] + ".txt";
		std::string newName = "empty_" + std::to_string(i);
		std::string newPath = bufferDir + "/" + newName + ".txt";

		if (fs::exists(oldPath) && buffer[i] != newName) {
			fs::rename(oldPath, newPath);
		}

		buffer[i] = newName;
	}

	buffer.clear();
}