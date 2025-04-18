#include "TestShellApp.h"
#include "TestShellDevice.h"

int main(int argc, char* argv[])
{
	TestShellApp app{ new TestShellDevice()};

	if (argc == 2) {
		app.runner(argv[1]);
	}
	else {
		app.run();
	}

	return 0;
}