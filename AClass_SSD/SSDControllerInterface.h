#pragma once
#include <vector>
#include "FileTextIOInterface.h"

class SSDControllerInterface {
public:
	SSDControllerInterface(FileTextIOInterface* data) : data(data) {};
	virtual void writeLBA(int lba, uint32_t value) = 0;
	virtual uint32_t readLBA(int lba) = 0;
protected:
	static const int LBA_SIZE = 100;
	std::vector<uint32_t> ssdData;
	FileTextIOInterface* data;
};