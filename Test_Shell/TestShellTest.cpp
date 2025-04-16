#include "gmock/gmock.h"
#include "TestShell.h"

TEST(TS, testRead) {
	TestShell ts;
	EXPECT_TRUE(ts.init("read 3 "));
}

TEST(TS, testWrite) {
	TestShell ts;
	EXPECT_TRUE(ts.init("write 3 0xAAAABBBB"));
}

TEST(TS, fullWrite) {
	TestShell testShell;
	uint32_t data = 0x12341234;
	int result = testShell.fullWrite(data);
	EXPECT_EQ(result, 0);
}

TEST(TS, write) {
	TestShell testShell;
	int lba = 0;
	uint32_t data = 0x12341234;
	int result = testShell.write(lba, data);
	EXPECT_EQ(result, 0);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}