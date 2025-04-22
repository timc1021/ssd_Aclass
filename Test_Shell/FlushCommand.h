#pragma once
#include "CommandBase.h"

class FlushCommand : public CommandBase {
public:
    FlushCommand(ITestShell* testShell) : CommandBase(testShell) {}
    COMMAND_RESULT execute(const std::vector<std::string>& commandToken) override {
        if (commandToken.size() != 1)
            ADD_LOG("FlushCommand::execute", "Warning, flush not need parameter");

        shell->flush();
        return COMMAND_SUCCESS;
    }

    bool isCommandValid(const std::vector<std::string>& commandToken) override { return true; }
};