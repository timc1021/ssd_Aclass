#include <gmock/gmock.h>

#include <string>
#include <exception>

#include "FileTextIOInterface.h"
#include "SSDController.h"

using namespace ::testing;

class DataMock : public FileTextIOInterface {
public:
	DataMock(std::string fileName) : FileTextIOInterface(fileName) {}
	MOCK_METHOD(std::string, loadFromFile, (), (override));
	MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};
class SSDControllerTestFixture : public Test {
public:
	std::string data = "0xFFFFFF10\n0x00000020\n";
	std::vector<uint32_t> data2Int = { 0xffffff10 ,0x00000020 };

	std::string emptyData = "";
	uint32_t emptyData2Int = 0x00000000;

	std::shared_ptr<DataMock> dataMock;
	SSDController ssd;

	SSDControllerTestFixture() : dataMock(std::make_shared <DataMock>("test.txt")), ssd(dataMock) {}
};

TEST_F(SSDControllerTestFixture, initSSDDataAndReadLBA) {

	EXPECT_CALL(*dataMock, loadFromFile()).WillRepeatedly(Return(data));

	EXPECT_EQ(ssd.readLBA(0), data2Int[0]);
	EXPECT_EQ(ssd.readLBA(1), data2Int[1]);
}
TEST_F(SSDControllerTestFixture, ReadLBAWhenDataIsEmpty) {

	EXPECT_CALL(*dataMock, loadFromFile()).WillRepeatedly(Return(emptyData));
	EXPECT_EQ(ssd.readLBA(0), emptyData2Int);
}
TEST_F(SSDControllerTestFixture, ThrowExceptionWhenReadLBAWithInvalidLBAValue) {

	EXPECT_CALL(*dataMock, loadFromFile()).WillRepeatedly(Return(emptyData));
	EXPECT_THROW({ ssd.readLBA(102); }, std::invalid_argument);
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