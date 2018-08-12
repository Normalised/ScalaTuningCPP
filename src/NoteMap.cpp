
#include "ScalaTuningCPP/NoteMap.h"

#include <cmath>
#include <utility>

namespace relivethefuture {
    
    NoteMap::NoteMap() {
        // create 12 TET Note Map
    }
    
    NoteMap::NoteMap(std::vector<double> ratios) {
        setRatios(ratios);
    }
    
    double NoteMap::getRatio(int noteNumber) {
        if(noteNumber < 0) noteNumber = 0;
        if(noteNumber >= noteToRatioMap.size()) {
            noteNumber = noteToRatioMap.size() - 1;
        }
        return noteToRatioMap[noteNumber];
    }
    
    double NoteMap::getRatio(int noteNumber, int pitchWheel) {
        double wheel = (double(pitchWheel) / 0x3FF);
        if(wheel > 0) {
            return getRatio(noteNumber + (pitchBendRangeUp * wheel));
        } else {
            return getRatio(noteNumber + (pitchBendRangeDown * wheel));
        }
    }

    double NoteMap::getRatio(double noteNumber) {
        if(noteNumber < 0) noteNumber = 0;
        if(noteNumber >= noteToRatioMap.size()) {
            noteNumber = noteToRatioMap.size() - 1;
        }
        const auto baseNoteNumber = std::floor(noteNumber);
        const auto nextNoteNumber = std::ceil(noteNumber);
        if(baseNoteNumber == nextNoteNumber) return noteToRatioMap[baseNoteNumber];
        
        const auto dn = noteNumber - baseNoteNumber;
        
        const auto first = noteToRatioMap[baseNoteNumber];
        const auto second = noteToRatioMap[nextNoteNumber];
        return first + ((second - first) * dn);
    }

    double NoteMap::getFrequency(int noteNumber) {
        return getRatio(noteNumber) * centerFrequency;
    }
    
    double NoteMap::getFrequency(double noteNumber) {
        return getRatio(noteNumber) * centerFrequency;
    }

    double NoteMap::getFrequency(int noteNumber, int pitchWheel) {
        return getRatio(noteNumber, pitchWheel) * centerFrequency;
    }

    void NoteMap::setRatios(std::vector<double> ratios) {
        const auto numRatios = ratios.size();
        if (numRatios > 2)
        {
            noteToRatioMap.clear();
            
            double octaveSize = ratios[numRatios];
            // Account for any errors in tuning files, reset octave size to default
            // if it's too weird.
            if (octaveSize <= 0.00000001)
            {
                octaveSize = 1.0;
            }
            const auto notesPerOctave = numRatios;
            
            double octaveBaseRatio = 0.0;
            int indexInOctave = 0;
            int octavesAway = 0;
            int relativeNoteIndex = 0;
            for (int i = 0; i < 128; i++)
            {
                relativeNoteIndex = i - centerNote;
                
                octavesAway = std::abs(std::floor(double(relativeNoteIndex) / double(notesPerOctave)));
                indexInOctave = relativeNoteIndex % notesPerOctave;
                const double octaveFactor = std::pow(octaveSize, octavesAway);
                
                if (relativeNoteIndex < 0)
                {
                    octaveBaseRatio = 1.0 / octaveFactor;
                    indexInOctave = (indexInOctave + notesPerOctave) % notesPerOctave;
                }
                else
                {
                    octaveBaseRatio = octaveFactor;
                }
                
                noteToRatioMap[i] = octaveBaseRatio * ratios[indexInOctave];
            }
        }
    }
    
    void NoteMap::resetTo12Tet() { 
        for(int i=0;i<128;i++)
        {
            noteToRatioMap[i] = std::pow(2.0, (i - centerNote) / 12.0);
        }
    }
    
    void NoteMap::setCenterNote(int noteNumber) {
        centerNote = noteNumber;
    }
    
    void NoteMap::setCenterFrequency(double freqInHz) {
        centerFrequency = freqInHz;
    }
    
    void NoteMap::setPitchBendRange(int up, int down) {
        pitchBendRangeUp = up;
        pitchBendRangeDown = down;
    }
    
    std::pair<int, int> NoteMap::getPitchBendRange() {
        return std::make_pair(pitchBendRangeUp, pitchBendRangeDown);
    }

}