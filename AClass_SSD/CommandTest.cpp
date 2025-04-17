#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Command.h"
#include "SSDControllerInterface.h"

class MockSSDController : public SSDControllerInterface {
public:
    MockSSDController(std::shared_ptr<FileTextIOInterface> data = nullptr) : SSDControllerInterface(std::move(data)) {}

    MOCK_METHOD(void, writeLBA, (int lba, unsigned int value), (override));
    MOCK_METHOD(unsigned int, readLBA, (int lba), (override));
};

class MockDataInterface : public FileTextIOInterface {
public:
    MockDataInterface(std::string fileName = "") : FileTextIOInterface(fileName) {}

    MOCK_METHOD(std::string, loadFromFile, (), (override));
    MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};

using namespace ::testing;

class CommandTest : public Test {
protected:
    std::shared_ptr<MockSSDController> mockSSD;
    std::shared_ptr<MockDataInterface> mockOutputFile;
    std::shared_ptr <Command> command;

    void SetUp() override {
        mockSSD = std::make_shared <MockSSDController>();
        mockOutputFile = std::make_shared <MockDataInterface>("ssd_output.txt");
        command = std::make_shared <Command>(mockSSD, mockOutputFile);
    }

};
TEST_F(CommandTest, ExecuteReadCommand_SavesFormattedOutput) {
    EXPECT_CALL(*mockSSD, readLBA(5))
        .Times(1)
        .WillOnce(Return(0x1234ABCD));

    EXPECT_CALL(*mockOutputFile, saveToFile("0x1234ABCD"))
        .Times(1);

    command->execute("R", 5);
}
TEST_F(CommandTest, ExecuteWriteCommand_CallsWriteLBA) {
    EXPECT_CALL(*mockSSD, writeLBA(7, 0xABCDEF01))
        .Times(1);

    command->execute("W", 7, "0xABCDEF01");
}
TEST_F(CommandTest, ExecuteInvalidCommand_ThrowsException) {
    EXPECT_THROW(command->execute("Z", 1), std::invalid_argument);
}
TEST_F(CommandTest, ExecuteWriteWithBadHexFormat_ThrowsException) {
    EXPECT_THROW(command->execute("W", 2, "12345678"), std::invalid_argument);
}