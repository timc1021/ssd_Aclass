#pragma once
#include "ICommand.h"
#include "CommandBase.h"
#include "ITestShell.h"

typedef enum {
	TOKEN_READ_LBA = 1,
	TOKEN_READ_NUM,
} READ_TOKEN;

class ReadCommand : public CommandBase {
public:
    ReadCommand(ITestShell* testShell) : CommandBase(testShell) {}        
    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override;
    bool isCommandValid(const std::vector<std::string>& commandToken) override;
};