#include <vector>
#include <string>
#include <cstdint>
#include "DataInterface.h"

class Data : public DataInterface {

public:
    Data(std::string fileName);

    std::string loadFromFile() override;
    void saveToFile(std::string data) const override;
};