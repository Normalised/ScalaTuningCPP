#ifndef NOTE_MAP_H
#define NOTE_MAP_H

#pragma once

#include <map>
#include <vector>

namespace relivethefuture {
    /**
     * @brief Note number to frequency and ratio mapper.
     *
     * Use this when you want to convert midi note numbers to either frequencies in Hz
     * or ratios suitable for samplers.
     *
     * The default NoteMap will create a 12 Tone Equal Temperament mapping (aka western standard tuning)
     *
     * For microtonal scales a NoteMap can be constructed directly with a set of ratios which define
     * the scale degrees in one octave, typically starting from 1 and ending on 2 (the next octave)
     *
     * You can set the first ratio to something other than 1 but Scala Tuning Format always assumes
     * that the first scale degree is 1/1
     *
     * The Octave size doesn't have to be 2, you can use any octave size you like (shrunk or stretched)
     * and the scale will repeat accordingly
     *
     * Any ratio sets with less than 127 entries will be repeated up and down the range
     * spreading from the center note.
     *
     * Ratio sets with more than 127 entries are available 'as is', they are not truncated to match
     * the 0-127 midi note range, so if you wish to divide one octave into 300 parts you can.
     *
     * The NoteMap defaults to C3 as the 'center note', i.e. the 1/1 ratio
     * The center frequency (C3) is 261.63 which gives A3 as 440hz
     *
     */
    class NoteMap {
    public:
        NoteMap();
        NoteMap(std::vector<double> ratios);

        /**
         *
         * @param noteNumber Probably a midi note number from 0, 127.
         * @return  frequency in Hz for this note
         */
        double getFrequency(int noteNumber) const;
        /**
         * @brief get note frequency with linear interpolation between note numbers.
         *
         * @param noteNumber    Will be truncated to
         * @return frequency in Hz
         */
        double getFrequency(double noteNumber) const;
        
        /**
         * @brief Use note number and pitch wheel to get note frequency.
         * Suitable for use directly with JUCE or similar that provides pitch wheel
         * as 14-bit value
         *
         * Use setPitchBendRange(up, down) to control how many scale degrees
         * the pitch wheel controls
         *
         * @param noteNumber
         * @param pitchWheel 14 bit pitch wheel value from 0 to 0x3fff
         * @return
         */
        double getFrequency(int noteNumber, int pitchWheel) const;

        /**
         * @brief Provides note ratio from note number, suitable for controlling
         * things like sampler playback rate.
         * For mappings with standard octave sizes :
         * Center note is 1
         * +1 octave is 2
         * -1 octave is 0.5
         *
         * @param noteNumber
         * @return note ratio.
         */
        double getRatio(int noteNumber) const;

        /**
         * @brief Note ratio with linear interpolation
         *
         * @param noteNumber
         * @return
         */
        double getRatio(double noteNumber) const;

        /**
         * @brief Note ratio with pitch wheel control.
         *
         * @param noteNumber
         * @param pitchWheel 14 bit pitch wheel value from 0 to 0x3fff
         * @return
         */
        double getRatio(int noteNumber, int pitchWheel) const;

        /**
         * @brief Get current pitch bend range
         * @return up and down range as a pair. first is up, second is down.
         */
        std::pair<int, int> getPitchBendRange() const;

        /**
         * @brief Reset the mapping with new ratios
         * @param ratios
         */
        void setRatios(std::vector<double> ratios);

        /**
         * Replace the internal mapping
         *
         * @param ratioMap
         */
        void setNoteToRatioMap(std::map<int, double> ratioMap);

        /**
         * @brief Set which note is 'Center', i.e. which note number
         * maps to the 1/1 ratio or the current Center Frequency
         * In most systems / hosts C3 is 60, although sometimes its 48.
         *
         * @param noteNumber New center note
         */
        void setCenterNote(int noteNumber);

        /**
         * @brief Set the frequency of the Center Note
         * Defaults to 261.63 for note number 60 (C3)
         * This gives A3 = 440hz.
         *
         * @param freqInHz
         */
        void setCenterFrequency(double freqInHz);

        /**
         * @brief How far the Pitch Wheel values will affect the output frequency or ratio
         *
         * @param up Pitch Bend range up in scale degrees (semitones in 12 tet)
         * @param down Pitch Bend range down in scale degrees (semitones in 12 tet)
         */
        void setPitchBendRange(int up, int down);

        /**
         * @brief Find out how many entries the mapper has. Typically 128
         * Can be more if configured with large ratio sets.
         *
         * @return The number of entries in the mapping.
         */
        int getMappingSize() const;

        /**
         * @brief Back To Safety!
         */
        void resetTo12Tet();

    private:
        // Mapping of note number to ratio
        std::map<int, double> noteToRatioMap;
        
        // Note number to be 1/1
        int centerNote = 60;
        // Frequency for center note
        // C3 = 261.63 gives A3 = 440
        double centerFrequency = 261.63;
        
        // Pitch bend range specified in scale degrees
        int pitchBendRangeUp = 12;
        int pitchBendRangeDown = 12;
    };
}

#endif
