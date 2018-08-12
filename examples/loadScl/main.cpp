
#include <ScalaTuningCPP/NoteMap.h>
#include <ScalaTuningCPP/ScalaTuning.h>

#include <iostream>

static inline std::string getExamplePath()
{
    std::string filePath(__FILE__);
    return filePath.substr( 0, filePath.length() - std::string("main.cpp").length());
}

static const std::string filename_riley_albion = getExamplePath() + "/riley_albion.scl";

int main() {

    relivethefuture::ScalaTuning scalaTuning;
    
    relivethefuture::NoteMap noteMap = scalaTuning.getNoteMapFromFile(filename_riley_albion);
    std::cout << "Note map loaded from file.\n";

    const double c3 = noteMap.getFrequency(60);
    std::cout << "C3 is " << c3 << "\n";

    std::cout << "Ratios from C3 to C4\n";
    for(int i=60;i<=72;i++) {
        std::cout << i << " -> " << noteMap.getRatio(i) << "\n";
    }
}
