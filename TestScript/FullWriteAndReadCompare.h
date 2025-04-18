#pragma once
#include <iostream>
#include "ITestScript.h"

class FullWriteAndReadCompare : public ITestScript {
public:
    std::vector<std::string> names() const override;
    void setShell(ITestShell* shell) override;
    bool run() override;


private:
    uint32_t getRandUint32();

    ITestShell* shell = nullptr;
}; 

extern "C" __declspec(dllexport) ITestScript* create();
