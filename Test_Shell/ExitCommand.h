#pragma once
#include "CommandBase.h"

class ExitCommand : public CommandBase {
public:
    ExitCommand(ITestShell* testShell) : CommandBase(testShell) {}
    COMMAND_RESULT execute(const std::vector<std::string>&) override {
        return COMMAND_EXIT;
    }
};