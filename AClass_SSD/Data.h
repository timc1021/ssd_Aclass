#pragma once
#include <vector>
#include <string>
#include <cstdint>

class Data {
private:
    static const int LBA_SIZE = 100;  // 총 LBA 수

public:
    std::vector<uint32_t> nandData;

    Data();  // 생성자: LBA 0으로 초기화

    bool loadFromFile(const std::string& filename);   // ssd_nand.txt 로드
    bool saveToFile(const std::string& filename) const; // ssd_nand.txt 저장
};
