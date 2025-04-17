#include "gmock/gmock.h"
#include "TestShellApp.h"
#include "TestShellMock.h"

using namespace testing;
using namespace std;

class TestShellFixture : public Test {
public:
	TestShellMock mock;
	TestShellApp app{ &mock };

	istringstream input;
	ostringstream output;
};

TEST_F(TestShellFixture, read) {
	EXPECT_CALL(mock, read(0))
		.Times(1)
		.WillOnce(Return(0));

	input.str("read 0");
	input.clear();
	app.run(input, output);
}

TEST_F(TestShellFixture, write) {
	EXPECT_CALL(mock, write(3, 0xAAAABBBB))
		.Times(1);

	input.str("write 3 0xAAAABBBB");
	input.clear();
	app.run(input, output);
}

TEST_F(TestShellFixture, fullRead) {
	EXPECT_CALL(mock, read(_))
		.Times(100)
		.WillRepeatedly(Return(0));

	input.str("fullRead");
	input.clear();
	app.run(input, output);
}

TEST_F(TestShellFixture, fullWrite) {
	EXPECT_CALL(mock, write(_, 0xAAAABBBB))
		.Times(100);

	input.str("fullWrite 0xAAAABBBB");
	input.clear();
	app.run(input, output);
}

TEST_F(TestShellFixture, invalidCommand) {
	input.str("invalidCommand");
	input.clear();
	app.run(input, output);
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, readWithInvalidLBA) {
	input.str("read 100");
	input.clear();
	app.run(input, output);
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, writeWithInvalidLBA) {
	input.str("write 100 0xAAAAAAAA");
	input.clear();
	app.run(input, output);
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, writeWithInvalidData) {
	input.str("write 1 invalid");
	input.clear();
	app.run(input, output);
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, readCompare) {
	int lba = 10;
	uint32_t expected = 0x1234abcd;

	EXPECT_CALL(mock, read(lba))
		.Times(1)
		.WillOnce(Return(expected));

	EXPECT_TRUE(mock.readCompare(lba, expected));
}

TEST_F(TestShellFixture, fullWriteAndReadCompare) {

	EXPECT_TRUE(mock.fullWriteAndReadCompare());
}

TEST_F(TestShellFixture, partialLBAWrite) {

	EXPECT_TRUE(mock.partialLBAWrite());
}

TEST_F(TestShellFixture, writeReadAging) {

	EXPECT_TRUE(mock.writeReadAging());
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}