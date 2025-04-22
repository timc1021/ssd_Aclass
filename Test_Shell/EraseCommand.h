#pragma once
#include "CommandBase.h"
#include "Type.h"

class EraseCommand : public CommandBase {
public:
    EraseCommand(ITestShell* testShell) : CommandBase(testShell) {}

    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override;
    bool isCommandValid(const std::vector<std::string>& commandToken) override;
};