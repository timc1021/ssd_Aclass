#pragma once
#include "ITestShell.h"
#include "gmock/gmock.h"

using std::string;
using std::vector;

class TestShellMock : public ITestShell {
public:
	MOCK_METHOD(void, write, (int lba, uint32_t data), (override));
	MOCK_METHOD(uint32_t, read, (int lba), (override));
	MOCK_METHOD(void, erase, (int lba, int size), (override));
	MOCK_METHOD(void, registerCommand, (const std::string& command, ITestScript* script), (override));
	MOCK_METHOD(bool, readCompareRange, (int start_lba, int end_lba, uint32_t data), (override));
	MOCK_METHOD(void, flush, (), (override));


	MOCK_METHOD(void, writeLBAs, (const vector<int>lba, const uint32_t data), (override));
	MOCK_METHOD(bool, readCompare, (const int lba, const uint32_t expected), (override));
	MOCK_METHOD(void, eraseRange, (const int startLba, const int endLba), (override));
};