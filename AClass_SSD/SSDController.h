#pragma once
#include <iostream>
#include <vector>

#include "DataInterface.h"
#include "SSDControllerInterface.h"

class SSDController : public SSDControllerInterface {

public:
	SSDController(DataInterface* data);
	void writeLBA(int lba, uint32_t value) override;
	uint32_t readLBA(int lba) override;
private:
	void getSsdDataFromFile();
	void saveSsdDataToFile();
};
