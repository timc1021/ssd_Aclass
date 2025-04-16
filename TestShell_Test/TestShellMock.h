#pragma once
#include "ITestShell.h"
#include "gmock/gmock.h"

using std::string;
using std::vector;

class TestShellMock : public ITestShell {
public:
	int handleCommand(string input) override;
	vector<string> splitBySpace(const string& input);
	int fullWrite(uint32_t data) override;
	uint32_t fullRead() override;
	int exit() override;
	void help() override;

	MOCK_METHOD(int, write, (int lba, uint32_t data), (override));
	MOCK_METHOD(uint32_t, read, (int lba), (override));
};