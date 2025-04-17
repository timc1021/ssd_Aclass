#pragma once
#include <iostream>
#include <vector>

#include "DataInterface.h"
#include "SSDControllerInterface.h"

class SSDController : public SSDControllerInterface {

public:
	SSDController(DataInterface* data);
	void writeLBA(int lba, unsigned int value) override;
	unsigned int readLBA(int lba) override;
private:
	void getSsdDataFromFile();
	void saveSsdDataToFile();
};
