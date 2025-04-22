
#include "CommandBuffer.h"
#include <gmock/gmock.h>

#include <filesystem>
#include "SSDControllerInterface.h"

using namespace ::testing;

namespace fs = std::filesystem;

class MockSSDController : public SSDControllerInterface {
public:
	MockSSDController(std::shared_ptr<FileTextIOInterface> data = nullptr)
		: SSDControllerInterface(std::move(data)) {
	}

	MOCK_METHOD(void, writeLBA, (int lba, uint32_t value), (override));
	MOCK_METHOD(uint32_t, readLBA, (int lba), (override));
};
class CommandBufferTest : public ::testing::Test {
public:
	std::shared_ptr<MockSSDController> mockSSD;
	void SetUp() override {
		mockSSD = std::make_shared<MockSSDController>();
	}
	void removeFilesAt(std::string dirPath) {
		if (!fs::exists(dirPath)) {
			return;
		}
		for (const auto& entry : fs::directory_iterator(dirPath)) {
			if (entry.is_regular_file()) {
				fs::remove(entry.path());
			}
		}
	}

};

TEST_F(CommandBufferTest, tc) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "W 20 0xABCDABCD";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 20 0xABCDABCD\n");
}

TEST_F(CommandBufferTest, tc2) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "W 20 0xABCDABCD";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 20 0xABCDABCD\n");

	strCommand = "W 20 0xFFFFFFFF";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 20 0xFFFFFFFF\n");
	//W 20 0xFFFFFFFF
}

TEST_F(CommandBufferTest, tc3) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "W 20 0xABCDABCD";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 20 0xABCDABCD\n");

	strCommand = "E 20 3";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 3\n");
	//E 20 3
}

TEST_F(CommandBufferTest, tc4) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 20 5";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 5\n");
	//E 20 5

	strCommand = "E 19 10";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 19 10\n");
	//E 19 10
}

TEST_F(CommandBufferTest, tc5) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 20 10";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 10\n");
	//E 20 10

	strCommand = "E 21 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 10\n");
	//E 20 10
}

TEST_F(CommandBufferTest, tc6) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 22 5";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 22 5\n");
	//E 22 5

	strCommand = "E 20 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 7\n");
	//E 20 7
}

TEST_F(CommandBufferTest, tc7) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 20 2";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 2\n");
	//E 20 2

	strCommand = "E 22 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 4\n");
	//E 20 4
}

TEST_F(CommandBufferTest, tc8) {
	removeFilesAt("./buffer");

	std::string strCommand = "E 20 3";
	CommandValue command(strCommand);
	CommandBuffer buffer(mockSSD);
	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 3\n");
	// E 20 3

	strCommand = "W 21 0x11112222";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 3\nW 21 0x11112222\n");
	// E 20 3
	// W 21 0x11112222

	strCommand = "E 22 3";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 5\nW 21 0x11112222\n");
	// E 20 5
	// W 21 0x1112222
}

TEST_F(CommandBufferTest, tc9) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 20 1";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 1\n");
	//E 20 1

	strCommand = "E 21 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 3\n");
	//E 20 3

	strCommand = "E 22 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 4\n");
	// E 20 4 (~23)

	strCommand = "E 25 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 25 2\nE 20 4\n");
	// E 20 4
	// E 25 2

	strCommand = "E 20 5";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 7\n");
	// E 20 7
}

TEST_F(CommandBufferTest, tc10) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 20 3";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 3\n");
	// E 20 3

	strCommand = "W 24 0x11112222";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 3\nW 24 0x11112222\n");
	// E 20 3
	// W 24 0x11112222

	strCommand = "E 22 5";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 7\n");
	// E 20 7
}

TEST_F(CommandBufferTest, tc11) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 10 3";
	CommandValue command(strCommand);

	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 10 3\n");
	// E 10 3

	strCommand = "E 15 3";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 15 3\nE 10 3\n");
	// E 10 3
	// E 15 3

	strCommand = "E 12 3";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 10 8\n");
	// E 10 8
}

TEST_F(CommandBufferTest, tc12) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 1 5";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 1 5\n");
	// E 1 5

	strCommand = "E 2 10";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 11 1\nE 1 10\n");
	// E 1 10
	// E 11 1
}

TEST_F(CommandBufferTest, tc13) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 10 3";
	CommandValue command(strCommand);

	EXPECT_CALL(*mockSSD, writeLBA(Ge(10), CommandValue::EMPTY_VALUE))
		.Times(3);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 10 3\n");
	// E 1 5

	buffer.flush();
	EXPECT_EQ(buffer.printBuffer(), "");
}

TEST_F(CommandBufferTest, tc14) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 95 5";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 5\n");
}

TEST_F(CommandBufferTest, tc15) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 89 5";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 89 5\n");

	strCommand = "E 92 8";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 99 1\nE 89 10\n");
}

TEST_F(CommandBufferTest, tc16) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 20 9";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 20 9\n");

	strCommand = "E 29 9";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 30 8\nE 20 10\n");

	strCommand = "E 38 10";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 40 8\nE 30 10\nE 20 10\n");

	strCommand = "E 46 10";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 50 6\nE 40 10\nE 30 10\nE 20 10\n");

	strCommand = "E 56 9";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 60 5\nE 50 10\nE 40 10\nE 30 10\nE 20 10\n");

	EXPECT_CALL(*mockSSD, writeLBA(Ge(20), CommandValue::EMPTY_VALUE))
		.Times(45);
	strCommand = "E 63 8";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	//EXPECT_EQ(buffer.printBuffer(), "E 60 5\nE 50 10\nE 40 10\nE 30 10\nE 20 10\n");
	EXPECT_EQ(buffer.printBuffer(), "E 63 8\n");
}

TEST_F(CommandBufferTest, tc17) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "W 99 0x11112222";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 99 0x11112222\n");

	strCommand = "E 29 10";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 29 10\nW 99 0x11112222\n");

	strCommand = "E 98 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 98 2\nE 29 10\n");

	strCommand = "W 99 0x11112222";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	//EXPECT_EQ(buffer.printBuffer(), "E 98 2\nE 29 10\nW 99 0x11112222\n");
	EXPECT_EQ(buffer.printBuffer(), "E 98 1\nE 29 10\nW 99 0x11112222\n");

	strCommand = "E 89 10";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 89 10\nE 29 10\nW 99 0x11112222\n");

	strCommand = "W 32 0xFFFFAAAA";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 89 10\nE 29 10\nW 99 0x11112222\nW 32 0xFFFFAAAA\n");

	strCommand = "E 39 10";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 89 10\nE 39 10\nE 29 10\nW 99 0x11112222\nW 32 0xFFFFAAAA\n");

	uint32_t result;
	buffer.getBufferedValueIfExists(29, result);
	EXPECT_EQ(result, 0x00000000);

	buffer.getBufferedValueIfExists(99, result);
	EXPECT_EQ(result, 0x11112222);
}

TEST_F(CommandBufferTest, tc18) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 99 1";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 99 1\n");

	strCommand = "W 99 0x11112222";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 99 0x11112222\n");
}

TEST_F(CommandBufferTest, tc19) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "W 1 0xFFFF1111";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 1 0xFFFF1111\n");

	strCommand = "E 10 2";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 10 2\nW 1 0xFFFF1111\n");

	strCommand = "W 10 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 11 1\nW 1 0xFFFF1111\nW 10 0xFFFF1111\n");
}

TEST_F(CommandBufferTest, tc20) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 95 5";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 5\n");

	strCommand = "W 97 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 5\nW 97 0xFFFF1111\n");

	strCommand = "W 96 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 5\nW 97 0xFFFF1111\nW 96 0xFFFF1111\n");

	strCommand = "W 95 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 98 2\nW 97 0xFFFF1111\nW 96 0xFFFF1111\nW 95 0xFFFF1111\n");
}

TEST_F(CommandBufferTest, tc21) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 95 5";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 5\n");

	strCommand = "W 98 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 5\nW 98 0xFFFF1111\n");

	strCommand = "W 97 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 5\nW 98 0xFFFF1111\nW 97 0xFFFF1111\n");

	strCommand = "W 99 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 2\nW 98 0xFFFF1111\nW 97 0xFFFF1111\nW 99 0xFFFF1111\n");
}

TEST_F(CommandBufferTest, tc22) {
	removeFilesAt("./buffer");
	CommandBuffer buffer(mockSSD);
	std::string strCommand = "E 95 3";
	CommandValue command(strCommand);

	buffer.printBuffer();
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 3\n");


	strCommand = "W 96 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 3\nW 96 0xFFFF1111\n");

	strCommand = "W 97 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "E 95 1\nW 96 0xFFFF1111\nW 97 0xFFFF1111\n");

	strCommand = "W 95 0xFFFF1111";
	command.setCommand(strCommand);
	buffer.addCommandToBuffer(command);
	EXPECT_EQ(buffer.printBuffer(), "W 96 0xFFFF1111\nW 97 0xFFFF1111\nW 95 0xFFFF1111\n");
}