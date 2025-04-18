#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "Command.h"
#include "SSDControllerInterface.h"
#include "FileTextIOInterface.h"

using namespace ::testing;

class MockSSDController : public SSDControllerInterface {
public:
	MockSSDController(std::shared_ptr<FileTextIOInterface> data = nullptr)
		: SSDControllerInterface(std::move(data)) {}

	MOCK_METHOD(void, writeLBA, (int lba, uint32_t value), (override));
	MOCK_METHOD(uint32_t, readLBA, (int lba), (override));
};

class MockDataInterface : public FileTextIOInterface {
public:
	MockDataInterface(std::string fileName = "")
		: FileTextIOInterface(fileName) {}

	MOCK_METHOD(std::string, loadFromFile, (), (override));
	MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};

class CommandTest : public Test {
protected:
	std::shared_ptr<MockSSDController> mockSSD;
	std::shared_ptr<MockDataInterface> mockOutputFile;
	std::unique_ptr<Command> command;

	void SetUp() override {
		auto dummyIO = std::make_shared<MockDataInterface>("test.txt");
		mockSSD = std::make_shared<MockSSDController>(dummyIO);
		mockOutputFile = std::make_shared<MockDataInterface>("output.txt");
		command = std::make_unique<Command>(mockSSD, mockOutputFile);
	}
};

TEST_F(CommandTest, ExecuteReadCommand_SavesFormattedOutput) {
	EXPECT_CALL(*mockSSD, readLBA(5)).WillOnce(Return(0x1234ABCD));
	EXPECT_CALL(*mockOutputFile, saveToFile("0x1234ABCD")).Times(1);
	command->execute("R", 5);
}

TEST_F(CommandTest, ExecuteWriteCommand_CallsWriteLBA) {
	EXPECT_CALL(*mockSSD, writeLBA(7, 0xABCDEF01)).Times(1);
	command->execute("W", 7, "0xABCDEF01");
}

TEST_F(CommandTest, ExecuteInvalidCommand_ThrowsException) {
	EXPECT_THROW(command->execute("Z", 1), std::invalid_argument);
}

TEST_F(CommandTest, ExecuteWriteWithBadHexFormat_ThrowsException) {
	EXPECT_THROW(command->execute("W", 2, "12345678"), std::invalid_argument);
}
