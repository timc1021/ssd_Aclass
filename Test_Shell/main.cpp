#include "TestShellApp.h"
#include "TestShell.h"

int main()
{
	TestShellApp app{ new TestShell()};

	app.run();

	return 0;
}