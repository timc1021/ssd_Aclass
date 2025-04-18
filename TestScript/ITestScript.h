#pragma once
#include <string>
#include <vector>

class ITestScript {
public:
    virtual ~ITestScript() {}
    virtual std::vector<std::string> names() const = 0;  
    virtual void run() = 0;
};