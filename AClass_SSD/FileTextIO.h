#include <vector>
#include <string>
#include <cstdint>
#include "FileTextIOInterface.h"

class FileTextIO : public FileTextIOInterface {

public:
	FileTextIO(std::string fileName);

	std::string loadFromFile() override;
	void saveToFile(const std::string data) const override;
};