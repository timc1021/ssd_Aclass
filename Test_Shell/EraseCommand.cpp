#include "EraseCommand.h"

COMMAND_RESULT EraseCommand::execute(const std::vector<std::string>& tokens)
{
    if (!isCommandValid(tokens)) {
        ADD_LOG("EraseCommand::execute", "ERROR, invalid parameter");
        return COMMAND_INVALID_PARAM;
    }        

    int lba = 0;
    int size = 0;
    tryParseInt32(tokens[TOKEN_ERASE_LBA], lba);
    tryParseInt32(tokens[TOKEN_ERASE_SIZE], size);

    if (size > 0) {

        int currentLba = lba;

        while (size > 0 && currentLba < MAX_LBA_SIZE) {
            int chunk = min(MAX_ERASE_LBA, size);
            chunk = min(chunk, MAX_LBA_SIZE - currentLba);

            shell->erase(currentLba, chunk);

            currentLba += chunk;
            size -= chunk;
        }
    }
    else {
        int absSize = -size;
        int currentLba = lba;

        while (absSize > 0 && currentLba >= 0) {
            int chunk = min({ MAX_ERASE_LBA, absSize, currentLba + 1 });
            shell->erase(currentLba - chunk + 1, chunk);

            currentLba -= chunk;
            absSize -= chunk;
        }
    }

    return COMMAND_SUCCESS;
}

bool EraseCommand::isCommandValid(const std::vector<std::string>& commandToken)
{
    int32_t lba = 0;
    int32_t size = 0;

    if (commandToken.size() != TOKEN_ERASE_NUM) return false;

    bool ret = tryParseInt32(commandToken[TOKEN_ERASE_LBA], lba);
    if (ret == false)
        return false;

    ret = tryParseInt32(commandToken[TOKEN_ERASE_SIZE], size);
    if (ret == false)
        return false;

    if (!isLBAValid(lba)) {   
        return false;
    }

    return true;
}
