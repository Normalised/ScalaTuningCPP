#pragma once

#include <map>
#include <vector>

namespace relivethefuture {
    class NoteMap {
    public:
        NoteMap();
        NoteMap(std::vector<double> ratios);
        
        double getFrequency(int noteNumber);
        double getFrequency(double noteNumber);
        
        // 14 bit pitch wheel value from 0 to 0x3fff
        double getFrequency(int noteNumber, int pitchWheel);
        
        double getRatio(int noteNumber);
        double getRatio(double noteNumber);
        
        // 14 bit pitch wheel value from 0 to 0x3fff
        double getRatio(int noteNumber, int pitchWheel);
        
        void setRatios(std::vector<double> ratios);
        
        void setNoteToRatioMap(std::map<int, double> ratioMap);
        void resetTo12Tet();
  
        void setCenterNote(int noteNumber);
        void setCenterFrequency(double freqInHz);
        void setPitchBendRange(int up, int down);
        std::pair<int, int> getPitchBendRange();
    private:
        std::map<int, double> noteToRatioMap;
        
        // Midi note to be 1/1
        int centerNote = 60;
        // Frequency for center note
        double centerFrequency = 440.0;
        
        // Pitch bend range specified in scale degrees
        // For 12 TET this is semitones.
        int pitchBendRangeUp = 12;
        int pitchBendRangeDown = 12;
    };
}
