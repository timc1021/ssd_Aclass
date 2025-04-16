#include "gmock/gmock.h"

#include "TestShell.cpp"

TEST(TS, testWrite) {
	TestShell ts;
	EXPECT_TRUE(ts.init("write 3 0xAAAABBBB"));
}

TEST(TS, testRead) {
	TestShell ts;
	EXPECT_TRUE(ts.init("read 3 "));
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}