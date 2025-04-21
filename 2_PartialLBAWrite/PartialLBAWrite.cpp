#include "ITestScript.h"
#include "ITestShell.h"
#include "PartialLBAWrite.h"

#define MAX_LBA_SIZE (100)

extern "C" __declspec(dllexport) ITestScript* create() {
	return new PartialLBAWrite();
}

std::vector<std::string> PartialLBAWrite::names() const {
	return { "2_", "2_PartialLBAWrite" };
}

void PartialLBAWrite::setShell(ITestShell* shell)
{
	this->shell = shell;
}

bool PartialLBAWrite::run()
{
	uint32_t data;
	vector<int> seq = { 4, 0, 3, 1, 2 };
	const int loop_count = 30;

	for (int loop = 0; loop < loop_count; loop++) {
		data = getRandUint32();

		shell->writeLBAs(seq, data);

		if (shell->readCompareRange(0, 4, data) == false) {
			cout << "PartialLBAWrite FAIL\n";
			return false;
		}
	}

	cout << "PartialLBAWrite PASS\n";
	return true;
}