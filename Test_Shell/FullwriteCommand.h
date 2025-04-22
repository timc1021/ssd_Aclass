#pragma once
#include "CommandBase.h"

class FullWriteCommand : public CommandBase {
public:
    FullWriteCommand(ITestShell* testShell) : CommandBase(testShell) {}

    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override;
    bool isCommandValid(const std::vector<std::string>& commandToken) override;
};