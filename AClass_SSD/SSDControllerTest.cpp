#include <gtest/gtest.h>
#include <memory>
#include "SSDController.h"
#include "FileTextIOInterface.h"

// 가짜 구현 (Mock이 아니라 Fake로 테스트)
class FileTextIOFake : public FileTextIOInterface {
public:
	std::string content;
	FileTextIOFake() : FileTextIOInterface("fake") {}

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
