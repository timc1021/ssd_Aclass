#include "Data.h"
#include "gtest/gtest.h"
#include <fstream>
#include <cstdio>  // remove()

class DataTest : public ::testing::Test {
protected:
    const std::string testFile = "test_file.txt";

    void TearDown() override {
        std::remove(testFile.c_str());  // �׽�Ʈ �� ���� ����
    }

    bool fileExists(const std::string& name) {
        std::ifstream f(name);
        return f.good();
    }
};

// �׽�Ʈ 1: ������ ���� �� loadFromFile�� �� ���ڿ� ��ȯ
TEST_F(DataTest, LoadFromNonexistentFileReturnsEmptyString) {
    Data d(testFile);
    std::string content = d.loadFromFile();
    EXPECT_EQ(content, "");
}

// �׽�Ʈ 2: ���� �� �ٽ� ������ ������ ������ ��ȯ
TEST_F(DataTest, SaveAndLoadConsistency) {
    Data d(testFile);
    std::string message = "SSD Emulator Test";

    d.saveToFile(message);
    std::string loaded = d.loadFromFile();

    EXPECT_EQ(loaded, message);
}

// �׽�Ʈ 3: ����� �׽�Ʈ
TEST_F(DataTest, OverwriteFileContent) {
    Data d(testFile);
    std::string first = "First Content";
    std::string second = "Newer Content";

    d.saveToFile(first);
    EXPECT_EQ(d.loadFromFile(), first);

    d.saveToFile(second);
    EXPECT_EQ(d.loadFromFile(), second);
}

// �׽�Ʈ 4: saveToFile�� ȣ���ص� ������ �����Ǵ��� Ȯ��
TEST_F(DataTest, SaveCreatesFileIfNotExists) {
    Data d(testFile);
    EXPECT_FALSE(fileExists(testFile));

    d.saveToFile("File created via saveToFile");
    EXPECT_TRUE(fileExists(testFile));
}
