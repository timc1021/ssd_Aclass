#pragma once
#include <iostream>
#include <vector>

#include "DataInterface.h"

class SSDData {
	static const int LBA_SIZE = 100;
	std::vector<unsigned int> ssdData;
	DataInterface* data;

public:
	SSDData(DataInterface* data);
	void writeLBA(int lba, unsigned int value);
	unsigned int readLBA(int lba);
	void getSsdData();
};
