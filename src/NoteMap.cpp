
#include "ScalaTuningCPP/NoteMap.h"

#include <cmath>
#include <utility>

namespace relivethefuture {
    
    NoteMap::NoteMap() {
        resetTo12Tet();
    }
    
    NoteMap::NoteMap(std::vector<double> ratios) {
        setRatios(ratios);
    }
    
    double NoteMap::getRatio(int noteNumber) const {
        if(noteNumber < 0) noteNumber = 0;
        if(noteNumber >= noteToRatioMap.size()) {
            noteNumber = noteToRatioMap.size() - 1;
        }
        return noteToRatioMap.at(noteNumber);
    }
    
    double NoteMap::getRatio(int noteNumber, int pitchWheel) const {
        double wheel = (double(pitchWheel) / 0x3FF);
        if(wheel > 0) {
            return getRatio(noteNumber + (pitchBendRangeUp * wheel));
        } else {
            return getRatio(noteNumber + (pitchBendRangeDown * wheel));
        }
    }

    double NoteMap::getRatio(double noteNumber) const {
        if(noteNumber < 0) noteNumber = 0;
        if(noteNumber >= noteToRatioMap.size()) {
            noteNumber = noteToRatioMap.size() - 1;
        }
        const auto baseNoteNumber = std::floor(noteNumber);
        const auto nextNoteNumber = std::ceil(noteNumber);
        if(baseNoteNumber == nextNoteNumber) return noteToRatioMap.at(baseNoteNumber);
        
        const auto dn = noteNumber - baseNoteNumber;
        
        const auto first = noteToRatioMap.at(baseNoteNumber);
        const auto second = noteToRatioMap.at(nextNoteNumber);
        return first + ((second - first) * dn);
    }

    double NoteMap::getFrequency(int noteNumber) const {
        return getRatio(noteNumber) * centerFrequency;
    }
    
    double NoteMap::getFrequency(double noteNumber) const {
        return getRatio(noteNumber) * centerFrequency;
    }

    double NoteMap::getFrequency(int noteNumber, int pitchWheel) const {
        return getRatio(noteNumber, pitchWheel) * centerFrequency;
    }

    void NoteMap::setRatios(std::vector<double> ratios) {
        const auto numRatios = ratios.size() - 1;
        if(numRatios < 2) {
            // TODO
            // Just octaves
        }
        else if (numRatios < 128)
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
        } else {
            noteToRatioMap.clear();
            // More than 128 ratios, just use them as is
            int i = 0;
            for(const auto & r : ratios) {
                noteToRatioMap[i++] = r;
            }
        }
    }
    
    void NoteMap::resetTo12Tet() {
        noteToRatioMap.clear();
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
    
    std::pair<int, int> NoteMap::getPitchBendRange() const {
        return std::make_pair(pitchBendRangeUp, pitchBendRangeDown);
    }

    void NoteMap::setNoteToRatioMap(std::map<int, double> ratioMap) {
        noteToRatioMap = ratioMap;
    }

    int NoteMap::getMappingSize() const {
        return noteToRatioMap.size();
    }
}
