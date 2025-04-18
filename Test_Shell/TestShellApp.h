#pragma once
#include <iostream>
#include "ITestShell.h"

class TestShellApp
{
public:
	TestShellApp(ITestShell* testShell) : testShell(testShell) {}

	void run(std::istream& in = std::cin, std::ostream& out = std::cout);
	void init();
	void runner(char* argv);

	ITestShell* testShell;
};