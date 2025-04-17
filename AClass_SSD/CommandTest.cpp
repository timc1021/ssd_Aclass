#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Command.h"
#include "SSDControllerInterface.h"

// SSDControllerInterface�� Mock Ŭ���� ����
class MockSSDController : public SSDControllerInterface {
public:
    MockSSDController(DataInterface* data = nullptr) : SSDControllerInterface(data) {}

    // writeLBA ���� �Լ� Mock ����
    MOCK_METHOD(void, writeLBA, (int lba, unsigned int value), (override));

    // readLBA ���� �Լ� Mock ����
    MOCK_METHOD(unsigned int, readLBA, (int lba), (override));
};

// DataInterface�� Mock Ŭ���� ����
class MockDataInterface : public DataInterface {
public:
    MockDataInterface(std::string fileName = "") : DataInterface(fileName) {}

    // ���Ͽ��� ���ڿ��� �д� �Լ� Mock ����
    MOCK_METHOD(std::string, loadFromFile, (), (override));

    // ���ڿ��� ���Ͽ� �����ϴ� �Լ� Mock ����
    MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};

using namespace ::testing;

// Command Ŭ���� �׽�Ʈ�� Test Fixture
class CommandTest : public Test {
protected:
    MockSSDController* mockSSD;                // SSDControllerInterface Mock ��ü
    MockDataInterface* mockOutputFile;         // DataInterface Mock ��ü
    Command* command;                          // �׽�Ʈ�� Command ��ü

    // �׽�Ʈ ���� �� ����
    void SetUp() override {
        mockSSD = new MockSSDController();
        mockOutputFile = new MockDataInterface("ssd_output.txt");
        command = new Command(*mockSSD, *mockOutputFile);
    }

    // �׽�Ʈ ���� �� ���ҽ� ����
    void TearDown() override {
        delete command;
        delete mockSSD;
        delete mockOutputFile;
    }
};

// �׽�Ʈ 1: Read ����� ����Ǹ� readLBA ȣ�� �� ���˵� ���� saveToFile�� ���޵Ǿ�� �Ѵ�
TEST_F(CommandTest, ExecuteReadCommand_SavesFormattedOutput) {
    EXPECT_CALL(*mockSSD, readLBA(5)).WillOnce(Return(0x1234ABCD));
    EXPECT_CALL(*mockOutputFile, saveToFile("0x1234ABCD"));

    command->execute("R", 5);
}

// �׽�Ʈ 2: Write ����� ����Ǹ� ��Ȯ�� LBA�� ������ writeLBA�� ȣ��Ǿ�� �Ѵ�
TEST_F(CommandTest, ExecuteWriteCommand_CallsWriteLBA) {
    EXPECT_CALL(*mockSSD, writeLBA(7, 0xABCDEF01));

    command->execute("W", 7, "0xABCDEF01");
}

// �׽�Ʈ 3: �������� �ʴ� ��ɾ� �Է� �� std::invalid_argument ���ܰ� �߻��ؾ� �Ѵ�
TEST_F(CommandTest, ExecuteInvalidCommand_ThrowsException) {
    EXPECT_THROW(command->execute("Z", 1), std::invalid_argument);
}

// �׽�Ʈ 4: �߸��� hex �������� Write ��� �� std::invalid_argument ���ܰ� �߻��ؾ� �Ѵ�
TEST_F(CommandTest, ExecuteWriteWithBadHexFormat_ThrowsException) {
    EXPECT_THROW(command->execute("W", 2, "12345678"), std::invalid_argument);
}
