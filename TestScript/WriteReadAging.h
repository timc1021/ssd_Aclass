#pragma once
#include <iostream>
#include "ITestScript.h"

class WriteReadAging : public ITestScript {
public:
    std::vector<std::string> names() const override
    {
        return { "3_", "3_WriteReadAging" };
    }

    void run() override { std::cout << "WriteReadAging ����!!\n"; } // ���� �Լ��� ; }
};


extern "C" __declspec(dllexport) ITestScript * create();
