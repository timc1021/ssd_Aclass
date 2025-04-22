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

		return 0;
	}

	std::ifstream file("ssd_output.txt");
	if (!file.is_open()) {
		ADD_LOG("ITestShell::read", "file open FAIL");
		std::cerr << "Failed to open ssd_output.txt" << std::endl;
		return 0;
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

bool TestShellDevice::readCompareRange(int start_lba, int end_lba, uint32_t data) {	
	bool result = false;

	for (int i = start_lba; i <= end_lba; i++) {
		result = readCompare(i, data);
		if (result == false)
			return result;
	}

	return result;
}

void TestShellDevice::writeLBAs(const vector<int>lba, const uint32_t data) {
	for (auto target_lba : lba) {
		write(target_lba, data);
	}
}

bool TestShellDevice::readCompare(int lba, const uint32_t expected) {
	int data;

	if (lba >= MAX_LBA_SIZE || lba < START_LBA) {
		return false;
	}

	data = read(lba);

	if (data == expected)
		return true;
	else {
		std::cout << "LBA " << lba << " : expected : 0x" << std::hex << expected << ", actual : 0x" << data << std::endl;
		return false;
	}
}

void TestShellDevice::eraseRange(const int startLba, const int endLba) {
	vector<string> cmd;
	cmd.push_back("erase_range");
	cmd.push_back(std::to_string(startLba));
	cmd.push_back(std::to_string(endLba));
	handleEraseRange(cmd);
}

void TestShellDevice::flush(void)
{
	std::ostringstream cmd;

	cmd << "SSD.exe F ";

	ADD_LOG("ITestShell::flush", cmd.str());
	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		ADD_LOG("ITestShell::flush", "FAIL CODE: " + std::to_string(retCode));
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;
	}
}
