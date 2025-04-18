#pragma once
#include <memory>
#include "SSDControllerInterface.h"

class SSDController : public SSDControllerInterface {
public:
	SSDController(std::shared_ptr<FileTextIOInterface> data);
	void writeLBA(int lba, uint32_t value) override;
	uint32_t readLBA(int lba) override;

private:
	void getSsdDataFromFile();
	void saveSsdDataToFile();
};