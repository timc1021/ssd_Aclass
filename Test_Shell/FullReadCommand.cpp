#include "FullReadCommand.h"

COMMAND_RESULT FullReadCommand::execute(const std::vector<std::string>&)
{
	ADD_LOG("FullReadCommand::execute", "read all LBAs");

	for (int lba = 0; lba < MAX_LBA_SIZE; lba++) {
		shell->read(lba);
	}

	return COMMAND_SUCCESS;
}

bool FullReadCommand::isCommandValid(const std::vector<std::string>& commandToken)
{
    return true;
}
