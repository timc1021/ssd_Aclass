#include "Data.h"
#include <fstream>
#include <sstream>

Data::Data(std::string fileName) : DataInterface(fileName) {}

std::string Data::loadFromFile() {
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        return "";
    }

    std::ostringstream ss;
    ss << inFile.rdbuf();
    return ss.str();
}

void Data::saveToFile(std::string data) const {
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        return;
    }

    outFile << data;
}
