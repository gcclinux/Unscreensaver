#include "headers/help.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <map>
#include "headers/colormap.hpp"

std::string getHelpText() {
    std::stringstream helpText;
    helpText << "Usage: ./screensaver [OPTIONS]\n\n"
             << "Options:\n"
             << "  --help                Show this help message\n"
             << "  --colour COLOR        Set background color (default: Black)\n"
             << "  --color COLOR         Same as --colour\n\n"
             << "\nAvailable colors:\n";
    
    int count = 0;
    for (auto it = COLOR_MAP.begin(); it != COLOR_MAP.end(); ++it) {
        helpText << " " << it->first;
        count++;
        if (count >= 10) {
            helpText << "\n";
            count = 0;
        } else if (std::next(it) != COLOR_MAP.end()) {
            helpText << ",";
        }
    }
    
    helpText << "\n\nPress ESC or \"Q\" to exit the screensaver\n";
    return helpText.str();
}