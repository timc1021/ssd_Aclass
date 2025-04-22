#include <iostream>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "TestShellApp.h"
#include "TestShellMock.h"
#include "Logger.h"
using namespace testing;
using namespace std;

class TestShellFixture : public Test {
public:

	TestShellFixture() {
		Logger::getInstance().setRunnerMode(true);
		EXPECT_CALL(mock, registerCommand(_, _)).Times(::testing::AnyNumber());
	}

	void executeRead(int lba)
	{
		string command = "read " + to_string(lba);
		executeCommand(command);
	}

	void executeWrite(int lba, int data)
	{
		ostringstream oss;
		oss << "write " << lba << " 0x" << hex << uppercase << data;
		string command = oss.str();
		executeCommand(command);
	}

	void executefullread() {
		string command = "fullread";
		executeCommand(command);
	}

	void executeFullwrite(int data) {
		ostringstream oss;
		oss << "fullwrite " << "0x" << hex << uppercase << data;
		string command = oss.str();
		executeCommand(command);
	}

	void executeErase(int lba, int size) {
		ostringstream oss;
		oss << "erase " << lba << " " << size;
		string command = oss.str();
		executeCommand(command);
	}

	void executeEraseRange(int startLba, int endLba) {
		ostringstream oss;
		oss << "erase_range " << startLba << " " << endLba;
		string command = oss.str();
		executeCommand(command);
	}

	void flush() {
		ostringstream oss;
		oss << "erase ";
		string command = oss.str();
		executeCommand(command);
	}

	void executeCommand(string command) {
		input.str(command);
		input.clear();
		app.run(input, output);
	}

	TestShellMock mock;
	TestShellApp app{ &mock };

	istringstream input;
	ostringstream output;
};

TEST_F(TestShellFixture, read) {
	EXPECT_CALL(mock, read(0))
		.Times(1)
		.WillOnce(Return(0));

	executeRead(0);
}

TEST_F(TestShellFixture, write) {
	EXPECT_CALL(mock, write(3, 0xAAAABBBB))
		.Times(1);

	executeWrite(3, 0xAAAABBBB);
}

TEST_F(TestShellFixture, fullRead) {
	EXPECT_CALL(mock, read(_))
		.Times(100)
		.WillRepeatedly(Return(0));

	executefullread();
}

TEST_F(TestShellFixture, fullWrite) {
	EXPECT_CALL(mock, write(_, 0xAAAABBBB))
		.Times(100);

	executeFullwrite(0xAAAABBBB);
}

TEST_F(TestShellFixture, erase) {
	EXPECT_CALL(mock, erase(1, 5))
		.Times(1);

	executeErase(1, 5);
}

TEST_F(TestShellFixture, eraseRange) {
	EXPECT_CALL(mock, erase(1, 6))
		.Times(1);

	executeEraseRange(1, 6);
}

TEST_F(TestShellFixture, eraseWithMultiChunck) {

	EXPECT_CALL(mock, erase(2, 10)).Times(1);
	EXPECT_CALL(mock, erase(12, 10)).Times(1);
	EXPECT_CALL(mock, erase(22, 10)).Times(1);
	EXPECT_CALL(mock, erase(32, 6)).Times(1);
	executeErase(2, 36);
}

TEST_F(TestShellFixture, eraseMultiChunckWithBigSize) {
	EXPECT_CALL(mock, erase(85, 10)).Times(1);
	EXPECT_CALL(mock, erase(95, 5)).Times(1);
	executeErase(85, 100);
}

TEST_F(TestShellFixture, eraseWithMultiChunckWithMinusSize) {
	EXPECT_CALL(mock, erase(0, 4)).Times(1);
	executeErase(3, -10);
}

TEST_F(TestShellFixture, eraseWithMultiChunckWithMinusSize2) {
	EXPECT_CALL(mock, erase(41, 10)).Times(1);
	EXPECT_CALL(mock, erase(38, 3)).Times(1);
	executeErase(50, -13);
}

TEST_F(TestShellFixture, eraseRangeMultiChunck) {
	EXPECT_CALL(mock, erase(0, 10)).Times(1);
	EXPECT_CALL(mock, erase(10, 10)).Times(1);
	EXPECT_CALL(mock, erase(20, 10)).Times(1);
	EXPECT_CALL(mock, erase(30, 10)).Times(1);
	EXPECT_CALL(mock, erase(40, 10)).Times(1);
	EXPECT_CALL(mock, erase(50, 10)).Times(1);
	EXPECT_CALL(mock, erase(60, 10)).Times(1);
	EXPECT_CALL(mock, erase(70, 10)).Times(1);
	EXPECT_CALL(mock, erase(80, 10)).Times(1);
	EXPECT_CALL(mock, erase(90, 10)).Times(1);
	executeEraseRange(0, 99);
}

TEST_F(TestShellFixture, eraseRangeSameStartWithEnd) {
	EXPECT_CALL(mock, erase(99, 1)).Times(1);
	executeEraseRange(99, 99);
	EXPECT_CALL(mock, erase(95, 1)).Times(1);
	executeEraseRange(95, 95);
}

TEST_F(TestShellFixture, eraseRangeWithInvalidParam) {
	executeCommand("erase_range");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase_range a b c");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase_range 40 10");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase_range -1 10");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase_range a");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase_range 1 200");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase_range 1 a");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase_range a a");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, eraseWithInvalidParam) {
	executeCommand("erase");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase -1");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase a");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("erase a 10");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, readWithInvalidParam) {
	executeCommand("read");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("read 100");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("read 1 1");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("read -1");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("read a");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, writeWithInvalidParam) {
	executeCommand("write");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("write 0xAAAAAAAA");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("write 100 0xAAAAAAAA");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("write 1 1234");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("write 1 abcd");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("write a 0xAAAAAAAA");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("write 55 0xAABBBB");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("write 3 0xAAAABBBB 2");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, fullwriteWithInvalidParam) {
	executeCommand("fullwrite");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("fullwrite 100 0xAAAAAAAA");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("fullwrite 1");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("fullwrite abcd");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("fullwrite 0xInvalid");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
	executeCommand("fullwrite 0 0");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}
TEST_F(TestShellFixture, fullreadWithInvalidParam) {
	executeCommand("fullread 0");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, invalidCommand) {
	executeCommand("invalidCommand");
	EXPECT_NE(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, parseExitCommand) {
	executeCommand("exit");
	EXPECT_EQ(std::string::npos, output.str().find("INVALID COMMAND"));
}

TEST_F(TestShellFixture, flushCommand) {
	EXPECT_CALL(mock, flush()).Times(1);
	executeCommand("flush");
	EXPECT_EQ(std::string::npos, output.str().find("INVALID COMMAND"));
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}