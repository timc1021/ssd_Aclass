#pragma once
#include <string>
#include <vector>
#include <memory>
#include "FileTextIO.h"

class CommandBuffer {
private:
	std::vector<std::string> buffer;
	const int maxBufferSize = 5;
	const std::string bufferDir = "./buffer";

	void loadInitialFiles();
	void createTxtFilesOnDestruction() const;

public:
	CommandBuffer();
	~CommandBuffer();

	void addCommandToBuffer(std::string command);
	void flush();
	void printBuffer() const;
};