#include "ScalaTuningCPP/ScalaTuning.h"

#include <cmath>

namespace relivethefuture {

    NoteMap ScalaTuning::getNoteMapFromFile(const std::string filename) {
        
        std::ifstream in(filename);
        if(!in.good()) {
            throw new std::invalid_argument("File not found");
        }
        std::string contents((std::istreambuf_iterator<char>(in)),
                             std::istreambuf_iterator<char>());
        
        const auto ratios = parse(contents);
        
        NoteMap noteMap(ratios);
        return noteMap;
    }
    
    std::vector<double> ScalaTuning::parse(std::string & tuning) {

        bool inComment = false;

        std::vector<double> ratios;

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
                        break;
                    }
                    if (numEntries == 0)
                    {
                        break;
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
                    const auto ratio = processRatioString(currentLine);
                    ratios.push_back(ratio);
                    currentLine = "";
                }
                else
                {
                    currentLine += character;
                }
            }
            if(ratios.size() > 127)
            {
                break;
            }
        }

        return ratios;

    }
    
    double ScalaTuning::processRatioString(std::string & ratioText)
    {
        double ratio = 0.0;
        
        if (ratioText.find(".") != std::string::npos)
        {
            const auto spacePos = ratioText.find(" ");
            double cents = 0.0;
            if (spacePos != std::string::npos)
            {
                std::string centsSubstring = ratioText.substr(0, spacePos);
                
                cents = std::stod(centsSubstring);
            }
            else
            {
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
            }
        }
        else
        {
            ratio = std::stod(ratioText);
        }
        return ratio;
    }

}
