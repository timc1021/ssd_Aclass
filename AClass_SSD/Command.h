#pragma once
#include "SSDControllerInterface.h"

class Command {
private:
    SSDControllerInterface& ssd;                // SSDData 객체를 참조하여 메모리와 데이터 처리
    DataInterface& outputFile;   // 출력 데이터를 저장할 DataInterface 객체 (파일 저장용)

public:
    Command(SSDControllerInterface& ssdData, DataInterface& outputFile);  // 생성자
    void execute(const std::string& cmdType, int lba, const std::string& valueHex = "");  // 명령어 처리 함수
};
