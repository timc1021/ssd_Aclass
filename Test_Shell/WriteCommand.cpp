#include "WriteCommand.h"

COMMAND_RESULT WriteCommand::execute(const std::vector<std::string>& tokens)
{
	if (!isCommandValid(tokens)) {
		ADD_LOG("WriteCommand::execute", "ERROR, invalid parameter");
		return COMMAND_INVALID_PARAM;
	}

	int lba = 0;
	tryParseInt32(tokens[TOKEN_WRITE_LBA], lba);

	uint32_t data = static_cast<uint32_t>(std::stoul(tokens[TOKEN_WRITE_DATA], nullptr, 0));
	shell->write(lba, data);

	return COMMAND_SUCCESS;
}

bool WriteCommand::isCommandValid(const std::vector<std::string>& commandToken)
{
	if (commandToken.size() != TOKEN_WRITE_NUM)
		return false;

	int32_t lba;
	bool ret = tryParseInt32(commandToken[TOKEN_WRITE_LBA], lba);
	if (ret == false)
		return false;

	if ((!isLBAValid(lba)) ||
		(!isDataValid(commandToken[TOKEN_WRITE_DATA]))) {
		return false;
	}

	return true;
}
