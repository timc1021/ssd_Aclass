#pragma once
#include "SSDControllerInterface.h"

class Command {
private:
    SSDControllerInterface& ssd;                // SSDData ��ü�� �����Ͽ� �޸𸮿� ������ ó��
    DataInterface& outputFile;   // ��� �����͸� ������ DataInterface ��ü (���� �����)

public:
    Command(SSDControllerInterface& ssdData, DataInterface& outputFile);  // ������
    void execute(const std::string& cmdType, int lba, const std::string& valueHex = "");  // ��ɾ� ó�� �Լ�
};
