#pragma once
#include "CommandBase.h"

class FlushCommand : public CommandBase {
public:
    FlushCommand(ITestShell* testShell) : CommandBase(testShell) {}
    COMMAND_RESULT execute(const std::vector<std::string>&) override {
        shell->flush();
        return COMMAND_SUCCESS;
    }
};