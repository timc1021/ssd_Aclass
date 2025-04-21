#pragma once
#include <iostream>
#include "ITestScript.h"

class PartialLBAWrite : public ITestScript {
public:
    std::vector<std::string> names() const override
    {
        return { "2_", "2_PartialLBAWrite" };
    }

    void run() override { std::cout << "PartialLBAWrite 수행!!\n"; } // 실제 함수콜 ; }
};


extern "C" __declspec(dllexport) ITestScript * create();
