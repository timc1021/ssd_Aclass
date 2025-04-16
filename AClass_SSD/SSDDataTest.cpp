#include <gmock/gmock.h>
#include <string>

#include "SSDData.h"
#include "DataInterface.h"

class DataMock : public DataInterface {
 public:
  DataMock(std::string fileName) : DataInterface(fileName) {}
  MOCK_METHOD(std::string, loadFromFile, (), (override));
  MOCK_METHOD(void, saveToFile, (std::string), (const, override));
};

TEST(SSDDataTest, initSSDDataAndReadLBA) {
  std::string data = "0xffffff10\n0x00000020";
  DataMock dataMock("test.txt");
  SSDData ssd(&dataMock);

  EXPECT_CALL(dataMock, loadFromFile()).WillRepeatedly(::testing::Return(data));

  EXPECT_EQ(ssd.readLBA(0), 0xffffff10);
  EXPECT_EQ(ssd.readLBA(1), 0x00000020);
}
