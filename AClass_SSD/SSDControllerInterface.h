#pragma once
#include <vector>
#include "DataInterface.h"

class SSDControllerInterface {
public:
	SSDControllerInterface(DataInterface* data) : data(data) {};
	virtual void writeLBA(int lba, unsigned int value) = 0;
	virtual unsigned int readLBA(int lba) = 0;
protected:
	static const int LBA_SIZE = 100;
	std::vector<unsigned int> ssdData;
	DataInterface* data;
};