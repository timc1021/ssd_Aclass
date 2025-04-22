#pragma once

#include "ICommand.h"
#include "CommandBase.h"
#include "ITestShell.h"

class FullReadCommand : public CommandBase {
public:
    FullReadCommand(ITestShell* testShell) : CommandBase(testShell) {}

    COMMAND_RESULT execute(const std::vector<std::string>&) override;
    bool isCommandValid(const std::vector<std::string>& commandToken) override;

};