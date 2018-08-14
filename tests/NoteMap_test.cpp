#include <ScalaTuningCPP/NoteMap.h>

#include <gtest/gtest.h>

TEST(NoteMap, default12Tet) {
    relivethefuture::NoteMap noteMap;
    EXPECT_EQ(1, noteMap.getRatio(60));
    EXPECT_EQ(261.63, noteMap.getFrequency(60));
}

TEST(NoteMap, albionRatios) {
    std::vector<double> albionRatios {
        1,
        16/15,
        9/8,
        6/5,
        5/4,
        4/3,
        64/45,
        3/2,
        8/5,
        5/3,
        16/9,
        15/8,
        2/1
    };
    
    relivethefuture::NoteMap albionNoteMap(albionRatios);
    
    EXPECT_EQ(16/15, albionNoteMap.getRatio(61));
}

