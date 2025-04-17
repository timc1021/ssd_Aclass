#include "Data.h"
#include <fstream>
#include <sstream>

// ������: ���� �̸� ����
Data::Data(std::string fileName) : DataInterface(fileName) {}

// ������ ���ڿ��� �ε�
std::string Data::loadFromFile() {
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        // ������ ���ų� �� �� ������ �� ���ڿ� ��ȯ
        return "";
    }

    std::ostringstream ss;
    ss << inFile.rdbuf();  // ��ü ���� �б�
    return ss.str();
}

// ���ڿ� �����͸� ���Ͽ� ����
void Data::saveToFile(std::string data) const {
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        // �����ص� ���ܴ� �߻����� ����
        return;
    }

    outFile << data;
}
