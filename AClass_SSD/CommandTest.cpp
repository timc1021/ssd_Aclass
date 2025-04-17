#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Command.h"
#include "SSDControllerInterface.h"

// SSDControllerInterface의 Mock 클래스 정의
class MockSSDController : public SSDControllerInterface {
public:
    MockSSDController(DataInterface* data = nullptr) : SSDControllerInterface(data) {}

    // writeLBA 가상 함수 Mock 정의
    MOCK_METHOD(void, writeLBA, (int lba, unsigned int value), (override));

    // readLBA 가상 함수 Mock 정의
    MOCK_METHOD(unsigned int, readLBA, (int lba), (override));
};

// DataInterface의 Mock 클래스 정의
class MockDataInterface : public DataInterface {
public:
    MockDataInterface(std::string fileName = "") : DataInterface(fileName) {}

    // 파일에서 문자열을 읽는 함수 Mock 정의
    MOCK_METHOD(std::string, loadFromFile, (), (override));

    // 문자열을 파일에 저장하는 함수 Mock 정의
    MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};

using namespace ::testing;

// Command 클래스 테스트용 Test Fixture
class CommandTest : public Test {
protected:
    MockSSDController* mockSSD;                // SSDControllerInterface Mock 객체
    MockDataInterface* mockOutputFile;         // DataInterface Mock 객체
    Command* command;                          // 테스트할 Command 객체

    // 테스트 실행 전 설정
    void SetUp() override {
        mockSSD = new MockSSDController();
        mockOutputFile = new MockDataInterface("ssd_output.txt");
        command = new Command(*mockSSD, *mockOutputFile);
    }

    // 테스트 종료 후 리소스 정리
    void TearDown() override {
        delete command;
        delete mockSSD;
        delete mockOutputFile;
    }
};

// 테스트 1: Read 명령이 수행되면 readLBA 호출 후 포맷된 값이 saveToFile에 전달되어야 한다
TEST_F(CommandTest, ExecuteReadCommand_SavesFormattedOutput) {
    EXPECT_CALL(*mockSSD, readLBA(5)).WillOnce(Return(0x1234ABCD));
    EXPECT_CALL(*mockOutputFile, saveToFile("0x1234ABCD"));

    command->execute("R", 5);
}

// 테스트 2: Write 명령이 수행되면 정확한 LBA와 값으로 writeLBA가 호출되어야 한다
TEST_F(CommandTest, ExecuteWriteCommand_CallsWriteLBA) {
    EXPECT_CALL(*mockSSD, writeLBA(7, 0xABCDEF01));

    command->execute("W", 7, "0xABCDEF01");
}

// 테스트 3: 존재하지 않는 명령어 입력 시 std::invalid_argument 예외가 발생해야 한다
TEST_F(CommandTest, ExecuteInvalidCommand_ThrowsException) {
    EXPECT_THROW(command->execute("Z", 1), std::invalid_argument);
}

// 테스트 4: 잘못된 hex 포맷으로 Write 명령 시 std::invalid_argument 예외가 발생해야 한다
TEST_F(CommandTest, ExecuteWriteWithBadHexFormat_ThrowsException) {
    EXPECT_THROW(command->execute("W", 2, "12345678"), std::invalid_argument);
}
