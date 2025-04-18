#include <gtest/gtest.h>
#include <memory>
#include "SSDController.h"
#include "FileTextIOInterface.h"

// 가짜 구현 (Mock이 아니라 Fake로 테스트)
class FileTextIOFake : public FileTextIOInterface {
public:
	DataMock(std::string fileName) : FileTextIOInterface(fileName) {}
	MOCK_METHOD(std::string, loadFromFile, (), (override));
	MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};
class SSDControllerTestFixture : public Test {
public:
	std::string data = "0xFFFFFF10\n0x00000020\n";
	std::vector<uint32_t> data2Int = { 0xffffff10 ,0x00000020 };

	std::string loadFromFile() override {
		return content;
	}

	void saveToFile(std::string data) const override {
		const_cast<FileTextIOFake*>(this)->content = data;
	}
};

TEST(SSDControllerTest, ReadEmptyDataReturnsZero) {
	auto fakeIO = std::make_shared<FileTextIOFake>();
	SSDController ssd(fakeIO);

	EXPECT_EQ(ssd.readLBA(0), 0x00000000);
}

TEST(SSDControllerTest, WriteThenReadReturnsSame) {
	auto fakeIO = std::make_shared<FileTextIOFake>();
	SSDController ssd(fakeIO);

	ssd.writeLBA(2, 0xAAAABBBB);
	EXPECT_EQ(ssd.readLBA(2), 0xAAAABBBB);
}
TEST_F(SSDControllerTestFixture, EmptySSDDataAndWriteLBA) {

	EXPECT_CALL(*dataMock, loadFromFile()).WillRepeatedly(Return(emptyData));
	EXPECT_CALL(*dataMock, saveToFile(StartsWith("0xFFFFFF10\n"))).Times(1);

	ssd.writeLBA(0, 0xffffff10);
}
TEST_F(SSDControllerTestFixture, NotEmptySSDDataAndWriteLBA) {

	EXPECT_CALL(*dataMock, loadFromFile()).WillRepeatedly(Return(data));
	EXPECT_CALL(*dataMock, saveToFile(StartsWith("0xFFFFFF10\0xFFFFFF10\n"))).Times(1);

	ssd.writeLBA(1, 0xffffff10);
}
TEST_F(SSDControllerTestFixture, ThrowExceptionWhenWriteLBAWithInvalidLBAValue) {

	EXPECT_THROW({ ssd.writeLBA(102, 0xffffff10); }, std::invalid_argument);
}