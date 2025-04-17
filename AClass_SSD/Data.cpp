#include "Data.h"
#include <fstream>
#include <sstream>

// 생성자: 파일 이름 저장
Data::Data(std::string fileName) : DataInterface(fileName) {}

// 파일을 문자열로 로드
std::string Data::loadFromFile() {
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        // 파일이 없거나 열 수 없으면 빈 문자열 반환
        return "";
    }

    std::ostringstream ss;
    ss << inFile.rdbuf();  // 전체 파일 읽기
    return ss.str();
}

// 문자열 데이터를 파일에 저장
void Data::saveToFile(std::string data) const {
    std::ofstream outFile(fileName);
    if (!outFile.is_open()) {
        // 실패해도 예외는 발생하지 않음
        return;
    }

    outFile << data;
}
