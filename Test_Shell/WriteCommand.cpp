#include "WriteCommand.h"

COMMAND_RESULT WriteCommand::execute(const std::vector<std::string>& tokens)
{
	if (tokens.size() != 3 || !isLBAValid(tokens[1]))
		return COMMAND_INVALID_PARAM;

	int lba = std::stoi(tokens[1]);
	uint32_t data = static_cast<uint32_t>(std::stoul(tokens[2], nullptr, 0));
	shell->write(lba, data);
	return COMMAND_SUCCESS;
}

bool WriteCommand::isWriteCommandValid(const vector<string> commandToken)
{
	if ((commandToken.size() != TOKEN_WRITE_NUM) ||
		(!isLBAValid(commandToken[TOKEN_WRITE_LBA])) ||
		(!isWriteDataValid(commandToken[TOKEN_WRITE_DATA]))) {
		ADD_LOG("ITestShell::isWriteCommandValid", "ERROR");

		return false;
	}

	return true;
}