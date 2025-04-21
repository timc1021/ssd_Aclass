#pragma once
#include "CommandBase.h"

class EraseCommand : public CommandBase {
public:
    EraseCommand(ITestShell* testShell) : CommandBase(testShell) {}

    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override {
        if (tokens.size() != 3 || !isLBAValid(tokens[1]))
            return COMMAND_INVALID_PARAM;

        int lba = std::stoi(tokens[1]);
        int size = std::stoi(tokens[2]);

        int currentLba = lba;
        while (size > 0 && currentLba < MAX_LBA_SIZE) {
            int chunk = std::min(MAX_ERASE_LBA, std::min(size, MAX_LBA_SIZE - currentLba));
            shell->erase(currentLba, chunk);
            currentLba += chunk;
            size -= chunk;
        }
        return COMMAND_SUCCESS;
    }
};

class EraseRangeCommand : public CommandBase {
public:
    EraseRangeCommand(ITestShell* testShell) : CommandBase(testShell) {}

    COMMAND_RESULT execute(const std::vector<std::string>& tokens) override {
        if (tokens.size() != 3 || !isLBAValid(tokens[1]) || !isLBAValid(tokens[2]))
            return COMMAND_INVALID_PARAM;

        int startLba = std::stoi(tokens[1]);
        int endLba = std::stoi(tokens[2]);
        if (startLba > endLba)
            return COMMAND_INVALID_PARAM;

        int size = endLba - startLba + 1;
        int currentLba = startLba;
        while (size > 0 && currentLba < MAX_LBA_SIZE) {
            int chunk = std::min(MAX_ERASE_LBA, std::min(size, MAX_LBA_SIZE - currentLba));
            shell->erase(currentLba, chunk);
            currentLba += chunk;
            size -= chunk;
        }
        return COMMAND_SUCCESS;
    }
};