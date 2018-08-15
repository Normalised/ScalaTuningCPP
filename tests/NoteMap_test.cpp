#include <ScalaTuningCPP/NoteMap.h>

#include <gtest/gtest.h>

TEST(NoteMap, default12Tet) {
    relivethefuture::NoteMap noteMap;
    EXPECT_EQ(1, noteMap.getRatio(60));
    EXPECT_EQ(261.63, noteMap.getFrequency(60));
}

TEST(NoteMap, albionRatios) {
    const std::vector<double> albionRatios {
        1,
        16.0/15.0,
        9.0/8.0,
        6.0/5.0,
        5.0/4.0,
        4.0/3.0,
        64.0/45.0,
        3.0/2.0,
        8.0/5.0,
        5.0/3.0,
        16.0/9.0,
        15.0/8.0,
        2.0/1.0
    };
    
    relivethefuture::NoteMap albionNoteMap(albionRatios);
    
    EXPECT_EQ(1.5, albionNoteMap.getRatio(67));
}

