#include <gmock/gmock.h>

#include <string>

#include "DataInterface.h"
#include "SSDData.h"

using namespace ::testing;

class DataMock : public DataInterface {
public:
	DataMock(std::string fileName) : DataInterface(fileName) {}
	MOCK_METHOD(std::string, loadFromFile, (), (override));
	MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};
class SSDDataTest : public Test {
public:
	std::string data = "0xffffff10\n0x00000020";
	std::vector<unsigned int> golden = { 0xffffff10 ,0x00000020 };
	DataMock dataMock;
	SSDData ssd;

	SSDDataTest() : dataMock("test.txt"), ssd(&dataMock) {}
};

TEST_F(SSDDataTest, initSSDDataAndReadLBA) {

	EXPECT_CALL(dataMock, loadFromFile()).WillRepeatedly(Return(data));

	EXPECT_EQ(ssd.readLBA(0), golden[0]);
	EXPECT_EQ(ssd.readLBA(1), golden[1]);
}
