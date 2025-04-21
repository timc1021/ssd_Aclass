#pragma once
#include <iostream>
#include <vector>
#include "ITestShell.h"

class TestShellApp
{
public:
	TestShellApp(ITestShell* testShell) : testShell(testShell) {}

	void run(std::istream& in = std::cin, std::ostream& out = std::cout);
	void runner(char* argv);
	bool init();

	ITestShell* testShell;
};