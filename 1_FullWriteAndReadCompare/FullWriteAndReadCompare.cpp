#include "ITestScript.h"
#include "FullWriteAndReadCompare.h"

#define MAX_LBA_SIZE (100)

extern "C" __declspec(dllexport) ITestScript* create() {
    return new FullWriteAndReadCompare();
}

std::vector<std::string> FullWriteAndReadCompare::names() const
{
	return { "1_", "1_FullWriteAndReadCompare" };
}

void FullWriteAndReadCompare::setShell(ITestShell* shell)
{
	this->shell = shell;
}

bool FullWriteAndReadCompare::run()
{
	uint32_t data;

	for (int lba_base = 0; lba_base < MAX_LBA_SIZE; lba_base += 5) {
		data = getRandUint32();

		for (int lba_offset = 0; lba_offset < 5; lba_offset++) {
			shell->write(lba_base + lba_offset, data);
		}

		if (shell->readCompareRange(lba_base, lba_base + 4, data) == false) {
			std::cout << "FullWriteAndReadCompare FAIL\n";
			return false;
		}
	}
	std::cout << "FullWriteAndReadCompare PASS\n";
	return true;
}
