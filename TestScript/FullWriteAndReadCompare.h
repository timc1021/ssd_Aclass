#pragma once
#include "ITestScript.h"

class FullWriteAndReadCompare : public ITestScript {
public:
    std::vector<std::string> names() const override
    { 
        return { "1_", "1_FullWriteAndReadCompare" }; 
    }

    void run() override {} // 실제 함수콜 ; }
};

extern "C" __declspec(dllexport) ITestScript* create() {
    return new FullWriteAndReadCompare();
}