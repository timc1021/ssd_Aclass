#include <iostream>
#include "TestShellDevice.h"

void TestShellDevice::write(int lba, uint32_t data)
{
	// TODO : system("ssd.exe");
	std::cout << "write done, lba : " << lba << ", data : " << data << std::endl;
}

uint32_t TestShellDevice::read(int lba)
{
	int result = 0;
	uint32_t read_data = 0;
	// result = system("ssd.exe");

	if (result == 0) {
		std::cout << "read done, lba : " << lba << ", data : " << read_data << std::endl;
	}
	else {
		return -1;
	}
	return read_data;
}
