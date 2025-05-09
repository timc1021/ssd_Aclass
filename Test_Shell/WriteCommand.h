#pragma once
#include "ICommand.h"
#include "CommandBase.h"
#include "ITestShell.h"

class WriteCommand : public CommandBase {
public:
    WriteCommand(ITestShell* testShell) : CommandBase(testShell) {}
    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override;
    bool isCommandValid(const std::vector<std::string>& commandToken) override;
};