#include <iostream>
#include "TestShellDevice.h"

void TestShellDevice::write(int lba, uint32_t data)
{
	// TODO : system("ssd.exe");
	std::cout << "[write] done\n";
}

uint32_t TestShellDevice::read(int lba)
{
	uint32_t readDate = 0;

	// result = system("ssd.exe");

	std::cout << "read done, lba : " << lba << ", data : " << readDate << std::endl;

	return readDate;
}
