#include "EraseRangeCommand.h"

COMMAND_RESULT EraseRangeCommand::execute(const std::vector<std::string>& tokens)
{
	if (!isCommandValid(tokens)) {
		ADD_LOG("EraseCommand::execute", "ERROR, invalid parameter");
		return COMMAND_INVALID_PARAM;
	}

	int startLba = std::stoi(tokens[TOKEN_ERASE_RANGE_START_LBA]);
	int endLba = std::stoi(tokens[TOKEN_ERASE_RANGE_END_LBA]);

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

bool EraseRangeCommand::isCommandValid(const std::vector<std::string>& commandToken)
{
	int32_t startLba = 0;
	int32_t endLba = 0;
	bool ret = tryParseInt32(commandToken[TOKEN_ERASE_RANGE_START_LBA], startLba);
	if (ret == false)
		return false;

	ret = tryParseInt32(commandToken[TOKEN_ERASE_RANGE_END_LBA], endLba);
	if (ret == false)
		return false;

	if ((commandToken.size() != TOKEN_ERASE_RANGNE_NUM) ||
		(!isLBAValid(startLba)) ||
		(!isLBAValid(endLba))) {
		return false;
	}

	return true;
}
