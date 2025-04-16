#include "gmock/gmock.h"
#include "TestShell.h"

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

TEST(TS, read) {
	TestShell testShell;
	int lba = 0;

	uint32_t result = testShell.read(lba);

	EXPECT_EQ(result, 0);
}

TEST(TS, fullRead) {
	TestShell testShell;

	int result = testShell.fullRead();

	EXPECT_EQ(result, 0);
}

TEST(TS, help) {
	TestShell testShell;

	int result = testShell.help();

	EXPECT_EQ(result, 0);
}

TEST(TS, handleReadCommand) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("read 3 "), 0);
}

TEST(TS, handleWriteCommand) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("write 3 0xAAAABBBB"), 0);
}

TEST(TS, handleWrongReadCommandParaNum) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("read"), -1);
}

TEST(TS, handleWrongWriteCommandParaNum) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("write 3 0xAAAABBBB 2"), -1);
}

TEST(TS, handleWrongReadLBA) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("read 200"), -1);
}

TEST(TS, handleWrongWriteLBA) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("write 300 0xAAAABBBB"), -1);
}

TEST(TS, handleWrongLengthWriteData) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("write 55 0xAABBBB"), -1);
}

TEST(TS, handleWrongRangeWriteData) {
	TestShell ts;
	EXPECT_EQ(ts.handleCommand("write 55 0xA!AABBBB"), -1);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}