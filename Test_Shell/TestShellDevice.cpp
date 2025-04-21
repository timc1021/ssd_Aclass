#include <iostream>
#include <sstream>
#include <fstream>
#include "TestShellDevice.h"
#include <iomanip>

void TestShellDevice::write(const int lba, const uint32_t data)
{
	std::ostringstream cmd;

	std::ostringstream oss;
	oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << data;
	cmd << "SSD.exe W " << lba << " " << oss.str();

	ADD_LOG("ITestShell::write", cmd.str());
	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		ADD_LOG("ITestShell::write", "FAIL CODE: " + std::to_string(retCode));
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;
	}
}

uint32_t TestShellDevice::read(const int lba)
{
	string strReadData;
	uint32_t readData = 0;

	std::ostringstream cmd;
	cmd << "SSD.exe R " << lba;

	ADD_LOG("ITestShell::read", cmd.str());
	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		ADD_LOG("ITestShell::read", "FAIL CODE: " + std::to_string(retCode));
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;

		return -1;
	}

	std::ifstream file("ssd_output.txt");
	if (!file.is_open()) {
		ADD_LOG("ITestShell::read", "file open FAIL");
		std::cerr << "Failed to open ssd_output.txt" << std::endl;
		return -2;
	}

	getline(file, strReadData);
	file.close();

	std::ostringstream oss;
	oss << "[read] LBA : " << std::right << std::setw(2) << std::setfill('0') << lba << ", DATA : " << strReadData << std::endl;
	ADD_LOG("ITestShell::read", oss.str());

	return stoul(strReadData, nullptr, 0);
}

void TestShellDevice::erase(const int lba, const int size)
{
	std::ostringstream cmd;
	cmd << "SSD.exe E " << lba << " " << size;

	ADD_LOG("ITestShell::erase", cmd.str());
	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		ADD_LOG("ITestShell::erase", "FAIL CODE : " + std::to_string(retCode));
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;
	}

	return;
}
