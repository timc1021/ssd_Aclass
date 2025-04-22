#pragma once
#include "CommandBase.h"

class ExitCommand : public CommandBase {
public:
    ExitCommand(ITestShell* testShell) : CommandBase(testShell) {}
    COMMAND_RESULT execute(const std::vector<std::string>& commandToken) override {
        if (commandToken.size() != 1)
            ADD_LOG("ExitCommand::execute", "Warning, exit not need parameter");

        return COMMAND_EXIT;
    }

    bool isCommandValid(const std::vector<std::string>& commandToken) override { return true; }
};