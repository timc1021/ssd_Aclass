#include "Data.h"
#include "gtest/gtest.h"
#include <fstream>
#include <cstdio>

class DataTest : public ::testing::Test {
protected:
    const std::string testFile = "test_file.txt";

    void TearDown() override {
        std::remove(testFile.c_str());
    }

    bool fileExists(const std::string& name) {
        std::ifstream f(name);
        return f.good();
    }
};
TEST_F(DataTest, LoadFromNonexistentFileReturnsEmptyString) {
    Data d(testFile);
    std::string content = d.loadFromFile();
    EXPECT_EQ(content, "");
}
TEST_F(DataTest, SaveAndLoadConsistency) {
    Data d(testFile);
    std::string message = "SSD Emulator Test";

    d.saveToFile(message);
    std::string loaded = d.loadFromFile();

    EXPECT_EQ(loaded, message);
}
TEST_F(DataTest, OverwriteFileContent) {
    Data d(testFile);
    std::string first = "First Content";
    std::string second = "Newer Content";

    d.saveToFile(first);
    EXPECT_EQ(d.loadFromFile(), first);

    d.saveToFile(second);
    EXPECT_EQ(d.loadFromFile(), second);
}
TEST_F(DataTest, SaveCreatesFileIfNotExists) {
    Data d(testFile);
    EXPECT_FALSE(fileExists(testFile));

    d.saveToFile("File created via saveToFile");
    EXPECT_TRUE(fileExists(testFile));
}
