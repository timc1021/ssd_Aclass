#pragma once
#include <iostream>
#include <vector>

#include "FileTextIOInterface.h"
#include "SSDControllerInterface.h"

class SSDController : public SSDControllerInterface {

public:
	SSDController(FileTextIOInterface* data);
	void writeLBA(int lba, uint32_t value) override;
	uint32_t readLBA(int lba) override;
private:
	void getSsdDataFromFile();
	void saveSsdDataToFile();
};
