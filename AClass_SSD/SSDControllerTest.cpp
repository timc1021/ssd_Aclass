#include <gmock/gmock.h>

#include <string>
#include <exception>

#include "DataInterface.h"
#include "SSDController.h"

using namespace ::testing;

class DataMock : public DataInterface {
public:
	DataMock(std::string fileName) : DataInterface(fileName) {}
	MOCK_METHOD(std::string, loadFromFile, (), (override));
	MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};
class SSDDataTest : public Test {
public:
	std::string data = "0xffffff10\n0x00000020\n";
	std::vector<unsigned int> data2Int = { 0xffffff10 ,0x00000020 };
	std::string emptyData = "";
	unsigned int emptyData2Int = 0x00000000;
	DataMock dataMock;
	SSDController ssd;

	SSDDataTest() : dataMock("test.txt"), ssd(&dataMock) {}
};

TEST_F(SSDDataTest, initSSDDataAndReadLBA) {

	EXPECT_CALL(dataMock, loadFromFile()).WillRepeatedly(Return(data));

	EXPECT_EQ(ssd.readLBA(0), data2Int[0]);
	EXPECT_EQ(ssd.readLBA(1), data2Int[1]);
}
TEST_F(SSDDataTest, ReadLBAWhenDataIsEmpty) {

	EXPECT_CALL(dataMock, loadFromFile()).WillRepeatedly(Return(emptyData));
	EXPECT_EQ(ssd.readLBA(0), emptyData2Int);
}
TEST_F(SSDDataTest, ThrowExceptionWhenReadLBAWithInvalidLBAValue) {

	EXPECT_CALL(dataMock, loadFromFile()).WillRepeatedly(Return(emptyData));
	EXPECT_THROW({ ssd.readLBA(102); }, std::invalid_argument);
}
TEST_F(SSDDataTest, EmptySSDDataAndWriteLBA) {

	EXPECT_CALL(dataMock, loadFromFile()).WillRepeatedly(Return(emptyData));
	EXPECT_CALL(dataMock, saveToFile(StartsWith("0xffffff10\n"))).Times(1);

	ssd.writeLBA(0, 0xffffff10);
}
TEST_F(SSDDataTest, NotEmptySSDDataAndWriteLBA) {

	EXPECT_CALL(dataMock, loadFromFile()).WillRepeatedly(Return(data));
	EXPECT_CALL(dataMock, saveToFile(StartsWith("0xffffff10\n0xffffff10\n"))).Times(1);

	ssd.writeLBA(1, 0xffffff10);
}
TEST_F(SSDDataTest, ThrowExceptionWhenWriteLBAWithInvalidLBAValue) {

	EXPECT_THROW({ ssd.writeLBA(102, 0xffffff10); }, std::invalid_argument);
}