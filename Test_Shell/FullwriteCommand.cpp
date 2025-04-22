#include "FullwriteCommand.h"

COMMAND_RESULT FullWriteCommand::execute(const std::vector<std::string>& tokens)
{
    ADD_LOG("ITestShell::fullWrite", "write all LBAs");

    if (!isCommandValid(tokens)) {
        ADD_LOG("FullWriteCommand::execute", "ERROR, invalid parameter");
        return COMMAND_INVALID_PARAM;
    }

    uint32_t data = static_cast<uint32_t>(std::stoul(tokens[1], nullptr, 0));

    for (int i = 0; i < MAX_LBA_SIZE; i++) {
        shell->write(i, data);
    }

    return COMMAND_SUCCESS;
}

bool FullWriteCommand::isCommandValid(const std::vector<std::string>& commandToken)
{
    if(commandToken.size() != TOKEN_FULLWRITE_NUM || !isDataValid(commandToken[TOKEN_FULLWRITE_DATA]))
        return false;

    return true;
}
