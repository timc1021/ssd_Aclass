#pragma once
#include "ITestShell.h"
#include "gmock/gmock.h"

using std::string;
using std::vector;

class TestShellMock : public ITestShell {
public:
	COMMAND_RESULT handleCommand(string commandLine) override;
	vector<string> splitBySpace(const string& input);
	void fullWrite(uint32_t data) override;
	uint32_t fullRead() override;
	COMMAND_RESULT exit() override;
	void help() override;

	MOCK_METHOD(void, write, (int lba, uint32_t data), (override));
	MOCK_METHOD(uint32_t, read, (int lba), (override));
};