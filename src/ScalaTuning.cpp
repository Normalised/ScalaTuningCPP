#include "ScalaTuningCPP/ScalaTuning.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace relivethefuture {

    std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
    {
        str.erase(0, str.find_first_not_of(chars));
        return str;
    }

    NoteMap ScalaTuning::getNoteMapFromFile(const std::string filename) {
        
        std::ifstream in(filename);
        if(!in.good()) {
            // TODO : Add C++17 version with new file exceptions
            throw std::invalid_argument("File not found");
        }
        std::string contents((std::istreambuf_iterator<char>(in)),
                             std::istreambuf_iterator<char>());
        
        std::vector<double> ratios;
        bool success = parse(contents, ratios);
        if(!success) {
            throw ParseException();
        }
        NoteMap noteMap(ratios);
        return noteMap;
    }
    
    bool ScalaTuning::parse(std::string & tuning, std::vector<double> & ratios) {

        bool inComment = false;

        int numEntries = 0;
        ratios.push_back(1.0);

        ScalaParseState state = ScalaParseState::DESCRIPTION_STATE;

        std::string currentLine = "";

        std::string description;

        for(char & character : tuning) {

            // If we're in a comment just keep reading until we hit the next line
            if (inComment)
            {
                if (character == '\n')
                {
                    inComment = false;
                }
                continue;
            }
            if (character == '!')
            {
                // Line is a comment, ignore until end
                inComment = true;
                if ((state == ScalaParseState::RATIO_STATE) && currentLine.length() > 1)
                {
                    // If a comment has started on a ratio line, we can process the ratio
                    const auto ratio = processRatioString(currentLine);
                    ratios.push_back(ratio);
                    currentLine = "";
                }
                continue;
            }
            else
            {
                inComment = false;
            }

            if (state == ScalaParseState::DESCRIPTION_STATE)
            {
                if (character == '\n')
                {
                    state = ScalaParseState::ENTRIES_STATE;
                    if (currentLine.length() == 0)
                    {
                        description = "No Info";
                    }
                    else
                    {
                        description = currentLine;
                    }

                    currentLine = "";
                }
                else
                {
                    currentLine += character;
                }
            }
            else if (state == ScalaParseState::ENTRIES_STATE)
            {
                if (character == '\n')
                {
                    state = ScalaParseState::RATIO_STATE;
                    try {
                        numEntries = std::stoi(currentLine);
                    } catch (std::exception & e) {
                        return false;
                    }
                    if (numEntries == 0)
                    {
                        return false;
                    }

                    currentLine = "";
                }
                else
                {
                    currentLine += character;
                }
            }
            else if (state == ScalaParseState::RATIO_STATE)
            {
                if (character == '\n')
                {
                    try {
                        const auto ratio = processRatioString(currentLine);
                        //std::cout << "Adding ratio " << ratio << std::endl;
                        ratios.push_back(ratio);
                        //std::cout << "Ratio count " << ratios.size() << std::endl;
                    } catch(const std::invalid_argument & e) {
                        //std::cout << "Invalid argument processing ratio string " << e.what() << std::endl;
                        return false;
                    }
                    currentLine = "";
                }
                else
                {
                    currentLine += character;
                }
            }
        }

        // ratios has the 1/1 entry, numEntries doesn't include that.
        if(numEntries != ratios.size() - 1) {
            //std::cout << "Num entries not equal to parsed ratios size. " << numEntries << " != " << ratios.size() << std::endl;
            return false;
        }
        if(state != ScalaParseState::RATIO_STATE) {
            return false;
        }
        return true;

    }
    
    double ScalaTuning::processRatioString(std::string & ratioText)
    {
        double ratio = 0.0;
        ratioText = ltrim(ratioText);
        
        if (ratioText.find(".") != std::string::npos)
        {
            const auto spacePos = ratioText.find(" ");
            double cents = 0.0;
            if (spacePos != std::string::npos)
            {
                //std::cout << "Space pos " << spacePos << ". Ratio Text " << ratioText << std::endl;
                std::string centsSubstring = ratioText.substr(0, spacePos);
                cents = std::stod(centsSubstring);
            }
            else
            {
                //std::cout << "Ratio Text " << ratioText << std::endl;
                cents = std::stod(ratioText);
            }
            
            ratio = pow(2, (cents / 100.0) / 12.0);
        }
        else if (ratioText.find("/") != std::string::npos)
        {
            const auto slashPos = ratioText.find("/");
            double numerator = std::stod(ratioText.substr(0, slashPos));
            double denominator = std::stod(ratioText.substr(slashPos + 1));
            if (numerator != 0 && denominator != 0)
            {
                ratio = numerator / denominator;
            } else {
                throw std::invalid_argument("Numerator or denominator is zero in " + ratioText);
            }
            //std::cout << "Ratio Text " << ratioText << std::endl;
        }
        else
        {
            ratio = std::stod(ratioText);
        }
        return ratio;
    }

}
