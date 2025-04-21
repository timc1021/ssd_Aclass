#include <iostream>
#include <sstream>
#include <fstream>
#include "TestShellDevice.h"
#include <iomanip>


void TestShellDevice::registerCommand(const std::string& command, ITestScript* script) {
	setScript(command, script);
}

void TestShellDevice::write(const int lba, const uint32_t data)
{
	std::ostringstream cmd;

	std::ostringstream oss;
	oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << data;
	cmd << "SSD.exe W " << lba << " " << oss.str();

	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;
	}
	else {
		std::cout << "[write] done" << std::endl;
	}
}

uint32_t TestShellDevice::read(const int lba)
{
	string strReadData;
	uint32_t readData = 0;

	std::ostringstream cmd;
	cmd << "SSD.exe R " << lba;

	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;

		return -1;
	}

	std::ifstream file("ssd_output.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open ssd_output.txt" << std::endl;
		return -2;
	}

	getline(file, strReadData);
	file.close();

	std::cout << "[read] LBA : " << std::right << std::setw(2) << std::setfill('0') << lba << " : " << strReadData << std::endl;

	return stoul(strReadData, nullptr, 0);
}

void TestShellDevice::erase(const int lba, const int size)
{
	std::ostringstream cmd;
	cmd << "SSD.exe E " << lba << " " << size;

	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;
	}

	return;
}

bool TestShellDevice::readCompareRange(int start_lba, int end_lba, uint32_t data) {	
	bool result = false;

	for (int i = start_lba; i <= end_lba; i++) {
		result = readCompare(i, data);
		if (result == false)
			return result;
	}

	return result;
}
