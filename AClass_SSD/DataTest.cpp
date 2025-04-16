#include "Data.h"
#include "gtest/gtest.h"
#include <fstream>
#include <cstdio>  // remove()

class DataTest : public ::testing::Test {
protected:
    const std::string testFile = "test_file.txt";

    void TearDown() override {
        std::remove(testFile.c_str());  // 테스트 후 파일 제거
    }

    bool fileExists(const std::string& name) {
        std::ifstream f(name);
        return f.good();
    }
};

// 테스트 1: 파일이 없을 때 loadFromFile은 빈 문자열 반환
TEST_F(DataTest, LoadFromNonexistentFileReturnsEmptyString) {
    Data d(testFile);
    std::string content = d.loadFromFile();
    EXPECT_EQ(content, "");
}

// 테스트 2: 저장 후 다시 읽으면 동일한 데이터 반환
TEST_F(DataTest, SaveAndLoadConsistency) {
    Data d(testFile);
    std::string message = "SSD Emulator Test";

    d.saveToFile(message);
    std::string loaded = d.loadFromFile();

    EXPECT_EQ(loaded, message);
}

// 테스트 3: 덮어쓰기 테스트
TEST_F(DataTest, OverwriteFileContent) {
    Data d(testFile);
    std::string first = "First Content";
    std::string second = "Newer Content";

    d.saveToFile(first);
    EXPECT_EQ(d.loadFromFile(), first);

    d.saveToFile(second);
    EXPECT_EQ(d.loadFromFile(), second);
}

// 테스트 4: saveToFile만 호출해도 파일이 생성되는지 확인
TEST_F(DataTest, SaveCreatesFileIfNotExists) {
    Data d(testFile);
    EXPECT_FALSE(fileExists(testFile));

    d.saveToFile("File created via saveToFile");
    EXPECT_TRUE(fileExists(testFile));
}
