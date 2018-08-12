#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "NoteMap.h"

namespace relivethefuture {
    /**
    * Scala files have 3 sections
    */
    enum class ScalaParseState
    {
        DESCRIPTION_STATE,
        ENTRIES_STATE,
        RATIO_STATE
    };

    class ScalaTuning {
    public:

        NoteMap getNoteMapFromFile(const std::string filename);

        std::vector<double> parse(std::string & tuningFileContents);
    private:

        /**
        * Take a single ratio string and convert to numeric ratio
        * Scala ratios can be either decimal or fractional
        * 0.33
        * 1/3
        * Are both valid ratio strings.
        */
        double processRatioString(std::string & ratioText);

    };
}
