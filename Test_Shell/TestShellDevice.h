#pragma once
#include "ITestShell.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class TestShellDevice : public ITestShell {
public:
	void write(int lba, uint32_t data);
	uint32_t read(int lba);
};