#include <vector>
#include <string>
#include <cstdint>

class Data {
private:
    std::string fileName;
public:
    Data(std::string fileName);

    std::string loadFromFile();
    void saveToFile(std::string data) const;
};