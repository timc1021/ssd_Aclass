#pragma once
#include "ITestShell.h"
#include "Logger.h"
#include <vector>
#include <string>
#include <unordered_map>
using std::string;
using std::vector;

class TestShellDevice : public ITestShell {
public:
	void write(const int lba, const uint32_t data) override;
	uint32_t read(const int lba) override;
	void erase(const int lba, const int size) override;
	void registerCommand(const std::string& command, ITestScript* script) override;
	bool readCompareRange(int start_lba, int end_lba, uint32_t data) override;
	void flush(void) override;
	void writeLBAs(const vector<int>lba, const uint32_t data) override;
	bool readCompare(int lba, const uint32_t expected) override;
	//void eraseRange(const int startLba, const int endLba) override;
};