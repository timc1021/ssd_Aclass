#pragma once
#include "ICommand.h"
#include "CommandBase.h"
#include "ITestShell.h"

class WriteCommand : public CommandBase {
public:
    WriteCommand(ITestShell* testShell) : CommandBase(testShell) {}
    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override;
    bool isWriteCommandValid(const vector<string> commandToken);
};


class FullWriteCommand : public CommandBase {
public:
    FullWriteCommand(ITestShell* testShell) : CommandBase(testShell) {}

    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override {
        if (tokens.size() != 2)
            return COMMAND_INVALID_PARAM;
        uint32_t data = static_cast<uint32_t>(std::stoul(tokens[1], nullptr, 0));
        shell->fullWrite(data);
        return COMMAND_SUCCESS;
    }
};