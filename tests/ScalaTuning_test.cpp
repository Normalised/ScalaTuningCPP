#include <ScalaTuningCPP/ScalaTuning.h>

#include <gtest/gtest.h>
#include <stdexcept>

static inline std::string getSclFilePath()
{
    std::string filePath(__FILE__);
    return filePath.substr( 0, filePath.length() - std::string("ScalaTuning_test.cpp").length());
}

static const std::string filename_harm6 = getSclFilePath() + "/../scala_files/harm6.scl";
static const std::string filename_fortune = getSclFilePath() + "/../scala_files/fortune.scl";
static const std::string filename_parse_error = getSclFilePath() + "/../scala_files/parse_error.scl";

TEST(ScalaTuning, loadSimpleScl) {
    relivethefuture::ScalaTuning scalaTuning;
    const auto noteMap = scalaTuning.getNoteMapFromFile(filename_harm6);

    EXPECT_EQ(1, noteMap.getRatio(60));
    EXPECT_EQ(9.0/8.0, noteMap.getRatio(61));
    EXPECT_EQ(5.0/4.0, noteMap.getRatio(62));
    EXPECT_EQ(11.0/8.0, noteMap.getRatio(63));
    EXPECT_EQ(3.0/2.0, noteMap.getRatio(64));
    EXPECT_EQ(7.0/4.0, noteMap.getRatio(65));
    EXPECT_EQ(2.0/1.0, noteMap.getRatio(66));

    EXPECT_EQ(261.63, noteMap.getFrequency(60));
}

TEST(ScalaTuning, loadSclThatsTooBigForMidi) {
    relivethefuture::ScalaTuning scalaTuning;
    const auto noteMap = scalaTuning.getNoteMapFromFile(filename_fortune);
    const int mapSize = noteMap.getMappingSize();
    EXPECT_EQ(613, mapSize);
    EXPECT_EQ(noteMap.getRatio(mapSize - 1), 2.0);
}

TEST(ScalaTuning, parseError) {
    relivethefuture::ScalaTuning scalaTuning;
    try {
        const auto noteMap = scalaTuning.getNoteMapFromFile(filename_parse_error);
        FAIL() << "Expected parse error";
    } catch(const relivethefuture::ParseException & e) {
        ASSERT_STREQ("Error parsing tuning", e.what());
    } catch(...) {
        FAIL() << "Wrong exception type";
    }
}

TEST(ScalaTuning, nonExistentFile) {
    relivethefuture::ScalaTuning scalaTuning;
    try {
        scalaTuning.getNoteMapFromFile("non_existent.scl");
        FAIL() << "Expect file not found";
    } catch(const std::invalid_argument & e) {
        ASSERT_STREQ("File not found", e.what());
    } catch(...) {
        FAIL() << "Uncaught exception / Unexpected exception type.";
    }
}
