#pragma once

#include <string>
#include <vector>

#include "ITestShell.h"

class ITestScript {
public:
    virtual ~ITestScript() {}
    virtual std::vector<std::string> names() const = 0;  
    virtual bool run() = 0;
    virtual void setShell(ITestShell* shell) = 0;
};

extern "C" __declspec(dllexport) ITestScript* create();