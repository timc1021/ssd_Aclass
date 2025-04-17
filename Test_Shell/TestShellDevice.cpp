#include <iostream>
#include <sstream>
#include <fstream>
#include "TestShellDevice.h"
#include <iomanip>

void TestShellDevice::write(int lba, uint32_t data)
{
	std::ostringstream cmd;

	std::ostringstream oss;
	oss << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << data;
	cmd << "SSD.exe W " << lba << " " << oss.str();

	// system call
	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;
	}
	else {
		std::cout << "[write] done" << std::endl;
	}
}

uint32_t TestShellDevice::read(int lba)
{
	string strReadData;
	uint32_t readData = 0;


	std::ostringstream cmd;
	cmd << "SSD.exe R " << lba;

	// system call
	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;

		return -1;
	}

	// 출력 파일 열기
	std::ifstream file("ssd_output.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open ssd_output.txt" << std::endl;
		return -2;
	}

	getline(file, strReadData);  // 한 줄만 읽고 싶을 경우
	file.close();

	std::cout << "[read] LBA : " << std::setw(2) << std::setfill('0') << lba << " : " << strReadData << std::endl;

	return stoi(strReadData, nullptr, 16);
}
