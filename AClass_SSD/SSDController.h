#pragma once
#include <iostream>
#include <vector>

#include "FileTextIOInterface.h"
#include "SSDControllerInterface.h"


class SSDController : public SSDControllerInterface {
public:
	SSDController(std::shared_ptr<FileTextIOInterface> data);

	void writeLBA(const int lba, const uint32_t value) override;
	uint32_t readLBA(const int lba) override;

private:
	void getSsdDataFromFile();
	void saveSsdDataToFile();
};
