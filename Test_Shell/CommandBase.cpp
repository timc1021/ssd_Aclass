#include "CommandBase.h"

bool CommandBase::isLBAValid(const std::string& lba)
{
	int iLba = std::stoi(lba);
	return (iLba >= 0 && iLba < MAX_LBA_SIZE);
}

bool CommandBase::isWriteDataValid(const string& writeData)
{
	if ((writeData.length() != DATA_LENGTH) ||
		(writeData.substr(0, HEX_PREFIX_LENGTH) != HEX_PREFIX)) {
		ADD_LOG("ITestShell::isWriteDataValid", "ERROR");

		return false;
	}

	for (char c : writeData.substr(HEX_PREFIX_LENGTH)) {
		if (isdigit(c) || isupper(c)) continue;

		ADD_LOG("ITestShell::isWriteDataValid", "ERROR");

		return false;
	}

	return true;
}