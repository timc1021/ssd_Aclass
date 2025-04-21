#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "SSDControllerInterface.h"
#include "FileTextIOInterface.h"
#include "CommandBuffer.h"

class ICommandExecutor {
public:
	virtual ~ICommandExecutor() = default;
	virtual void execute(const std::string& cmdType, int lba, const std::string& valueHex = "") = 0;
};

class Command : public ICommandExecutor {
private:
	std::shared_ptr<SSDControllerInterface> ssd;
	std::shared_ptr<FileTextIOInterface> outputFile;
	std::shared_ptr<CommandBuffer> buffer;

	std::unordered_map<std::string, std::function<void(int, const std::string&)>> commandMap;

	void registerCommands();

public:
	Command(std::shared_ptr<SSDControllerInterface> ssd,
		std::shared_ptr<FileTextIOInterface> outputFile,
		std::shared_ptr<CommandBuffer> buffer);

	void execute(const std::string& cmdType, int lba, const std::string& valueHex = "") override;
};
