#include "ITestScript.h"
#include "ITestShell.h"
#include "WriteReadAging.h"

extern "C" __declspec(dllexport) ITestScript* create() {
	return new WriteReadAging();
}

std::vector<std::string> WriteReadAging::names() const {
	return { "3_", "3_WriteReadAging" };
}

void WriteReadAging::setShell(ITestShell* shell)
{
	this->shell = shell;
}

bool WriteReadAging::run()
{
	uint32_t data = 0;
	vector<int>lba = { 0, 99 };
	const int loop_count = 200;

	for (int loop = 0; loop < loop_count; loop++) {
		data = getRandUint32();

		shell->writeLBAs(lba, data);

		if (shell->readCompare(lba[0], data) == false || shell->readCompare(lba[1], data) == false) {
			cout << "WriteReadAging FAIL\n";
			return false;
		}
	}
	cout << "WriteReadAging PASS\n";
	return true;
}
