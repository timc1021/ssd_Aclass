#include "ReadCommand.h"

COMMAND_RESULT ReadCommand::execute(const std::vector<std::string>& tokens)
{
	if (!isCommandValid(tokens)) {
		ADD_LOG("ReadCommand::execute", "ERROR, invalid parameter");
		return COMMAND_INVALID_PARAM;
	}

	int32_t lba;
	bool ret = tryParseInt32(tokens[TOKEN_READ_LBA], lba);
	if (ret == false)
		return COMMAND_INVALID_PARAM;

	shell->read(lba);

	return COMMAND_SUCCESS;
}

bool ReadCommand::isCommandValid(const std::vector<std::string>& commandToken)
{
	if (commandToken.size() != TOKEN_READ_NUM)
		return false;

	int32_t lba;
	bool ret = tryParseInt32(commandToken[TOKEN_READ_LBA], lba);
	if (ret == false)
		return false;

	if (!isLBAValid(lba))
		return false;

	return true;
}
