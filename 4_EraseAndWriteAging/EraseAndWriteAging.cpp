#include "EraseAndWriteAging.h"

extern "C" __declspec(dllexport) ITestScript* create() {
	return new EraseAndWriteAging();
}

std::vector<std::string> EraseAndWriteAging::names() const
{
    return { "4_", "4_EraseAndWriteAging" };
}

bool EraseAndWriteAging::run()
{
	uint32_t write_data = getRandUint32();
	uint32_t overwrite_data = getRandUint32();
	const int loop_count = 30;

	shell->erase(0, 3);

	for (int loop = 0; loop < loop_count; loop++) {
		for (int lba_base = 2; lba_base + 2 < 100; lba_base += 2) {
			shell->write(lba_base, write_data);
			shell->write(lba_base, overwrite_data);

			shell->erase(lba_base, 3);

			if (shell->readCompareRange(lba_base, lba_base + 2, 0) == false) {
				return false;
			}
		}
	}
	return true;
}

void EraseAndWriteAging::setShell(ITestShell* shell)
{
	this->shell = shell;
}
