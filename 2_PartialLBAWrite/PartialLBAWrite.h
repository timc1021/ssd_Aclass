#pragma once
#include <iostream>
#include "ITestScript.h"

class PartialLBAWrite : public ITestScript {
public:
    std::vector<std::string> names() const override;
    bool run() override;
    void setShell(ITestShell* shell) override;
};

extern "C" __declspec(dllexport) ITestScript * create();
