#include "CommandBase.h"

bool CommandBase::isLBAValid(int32_t lba)
{
	return (lba >= 0 && lba < MAX_LBA_SIZE);
}

bool CommandBase::isDataValid(const string& data)
{
	if ((data.length() != DATA_LENGTH) ||
		(data.substr(0, HEX_PREFIX_LENGTH) != HEX_PREFIX)) {
		ADD_LOG("CommandBase::isDataValid", "ERROR");

		return false;
	}

	for (char c : data.substr(HEX_PREFIX_LENGTH)) {
		if (isdigit(c) || isupper(c)) continue;

		ADD_LOG("CommandBase::isDataValid", "ERROR");

		return false;
	}

	return true;
}

bool CommandBase::tryParseInt32(const std::string& str, int32_t& outValue) {
    try {
        size_t pos = 0;
        long value = std::stol(str, &pos, 10);

        if (pos != str.size() || value < INT32_MIN || value > INT32_MAX) {
            return false;
        }

        outValue = static_cast<int32_t>(value);
        return true;
    }
    catch (...) {
        return false;
    }
}