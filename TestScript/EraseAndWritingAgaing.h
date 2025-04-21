#pragma once
#include <iostream>
#include "ITestScript.h"

class EraseAndWriteAging : public ITestScript {
public:
    std::vector<std::string> names() const override
    {
        return { "4_", "4_EraseAndWriteAging" };
    }

    void run() override { std::cout << "EraseAndWriteAging 수행!!\n"; } // 실제 함수콜 ; }
};


extern "C" __declspec(dllexport) ITestScript * create();
