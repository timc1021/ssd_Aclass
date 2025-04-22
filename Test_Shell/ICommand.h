#pragma once
#include <vector>
#include <string>
#include "Type.h"

class ITestShell;

class ICommand {
public:
	virtual COMMAND_RESULT execute(const std::vector<std::string>& commandToken) = 0;
	virtual bool isCommandValid(const std::vector<std::string>& commandToken) = 0;
};
