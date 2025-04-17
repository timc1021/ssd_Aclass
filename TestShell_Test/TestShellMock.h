#pragma once
#include "ITestShell.h"
#include "gmock/gmock.h"

using std::string;
using std::vector;

class TestShellMock : public ITestShell {
public:
	MOCK_METHOD(void, write, (int lba, uint32_t data), (override));
	MOCK_METHOD(uint32_t, read, (int lba), (override));
};