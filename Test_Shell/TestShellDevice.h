#pragma once
#include "ITestShell.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

class TestShellDevice : public ITestShell {
public:
	void write(const int lba, const uint32_t data);
	uint32_t read(const int lba);
};