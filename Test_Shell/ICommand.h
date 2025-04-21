#pragma once
#include <vector>
#include <string>
#include "Types.h"

class ITestShell;

class ICommand {
public:
	virtual COMMAND_RESULT execute(const std::vector<std::string>& commandToken) = 0;
};
