#include <iostream>
#include <sstream>
#include <fstream>
#include "TestShellDevice.h"

void TestShellDevice::write(int lba, uint32_t data)
{
	std::ostringstream cmd;
	cmd << "SSD.exe W " << lba << " " << data;

	// system call
	int retCode = std::system(cmd.str().c_str());
	if (retCode != 0) {
		std::cerr << "Failed to run command: " << cmd.str() << ", return code: " << retCode << std::endl;
	}
	else {
		std::cout << "write done, lba : " << lba << ", data : " << data << std::endl;
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

	// ��� ���� ����
	std::ifstream file("ssd_output.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open ssd_output.txt" << std::endl;
		return -2;
	}

	getline(file, strReadData);  // �� �ٸ� �а� ���� ���
	file.close();


	std::cout << "read done, lba : " << lba << ", data : " << strReadData << std::endl;
	
	return stoi(strReadData, nullptr, 16);
}
