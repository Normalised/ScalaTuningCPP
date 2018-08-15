#ifndef SCALA_TUNING_H
#define SCALA_TUNING_H

#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "NoteMap.h"

namespace relivethefuture {
    /**
     * @brief Exception thrown when a Scala Tuning file can't be parsed
     */
    struct ParseException: public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Error parsing tuning";
        }
    };
    /**
     * @brief Scala Parse State
     * Current state of the parser. Scala files have 3 sections : Description, Entries (integer) and a list of ratios
     */
    enum class ScalaParseState
    {
        DESCRIPTION_STATE,
        ENTRIES_STATE,
        RATIO_STATE
    };

    /**
     * @brief Scala Tuning file loader and parser. Converts .scl files into NoteMap objects which can
     * be used to map midi note numbers to both frequencies in Hz as well as ratios.
     */
    class ScalaTuning {
    public:

        /**
         * @brief Read and parse the file from the supplied filename
         *
         * @param filename  name of scala tuning file in .scl format
         *
         * @throws std::invalid_argument, ParseException
         *
         * @return NoteMap
         */
        NoteMap getNoteMapFromFile(const std::string filename);

        /**
         * @brief take a scala tuning file contents as a string and populate the supplied vector of ratios
         *
         * @param tuningFileContents    complete contents of a scala tuning file
         * @param ratios    vector of doubles to fill with calculated ratios from the tuning file. Any existing contents will be lost.
         *
         * @return true if parse is successful, false for parse failure.
         */
        bool parse(std::string & tuningFileContents, std::vector<double> & ratios);
        
    private:

        /**
         * @brief Take a single ratio string and convert to numeric ratio
         *
         * @param ratioText     string representation of a scala ratio
         * Scala ratios can be either decimal or fractional e.g 0.33 and 1/3 are both valid ratio strings.
         *
         * @return ratio as double
         */
        double processRatioString(std::string & ratioText);

    };
}

#endif
