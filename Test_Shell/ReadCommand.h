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
        
    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override {
        if (tokens.size() != TOKEN_READ_NUM || !isLBAValid(tokens[TOKEN_READ_LBA]))
            return COMMAND_INVALID_PARAM;
        shell->read(std::stoi(tokens[1]));
        return COMMAND_SUCCESS;
    }
};

class FullReadCommand : public CommandBase {
public:
    FullReadCommand(ITestShell* testShell) : CommandBase(testShell) {}

    COMMAND_RESULT execute(const std::vector<std::string>&) override {
        shell->fullRead();
        return COMMAND_SUCCESS;
    }
};