#pragma once
#include <vector>
#include <string>
#include <cstdint>

class Data {
private:
    static const int LBA_SIZE = 100;  // �� LBA ��

public:
    std::vector<uint32_t> nandData;

    Data();  // ������: LBA 0���� �ʱ�ȭ

    bool loadFromFile(const std::string& filename);   // ssd_nand.txt �ε�
    bool saveToFile(const std::string& filename) const; // ssd_nand.txt ����
};
