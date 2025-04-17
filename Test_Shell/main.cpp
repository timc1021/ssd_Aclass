#include "TestShellApp.h"
#include "TestShellDevice.h"

int main()
{
	TestShellApp app{ new TestShellDevice()};

	app.run();

	return 0;
}