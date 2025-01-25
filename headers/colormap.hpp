#ifndef COLORMAP_HPP
#define COLORMAP_HPP

#include <map>
#include <string>
#include <SFML/Graphics/Color.hpp>

const std::map<std::string, sf::Color> COLOR_MAP = {
    {"Black", sf::Color::Black},
    {"White", sf::Color::White},
    {"Red", sf::Color::Red},
    {"Green", sf::Color::Green},
    {"Blue", sf::Color::Blue},
    {"Yellow", sf::Color::Yellow},
    {"Magenta", sf::Color::Magenta},
    {"Cyan", sf::Color::Cyan},
    {"Orange", sf::Color(255, 165, 0)},
    {"Pink", sf::Color(255, 192, 203)},
    {"Purple", sf::Color(128, 0, 128)},
    {"Brown", sf::Color(165, 42, 42)},
    {"Grey", sf::Color(128, 128, 128)},
    {"LightGrey", sf::Color(211, 211, 211)},
    {"DarkGrey", sf::Color(169, 169, 169)},
    {"LightBlue", sf::Color(173, 216, 230)},
    {"LightGreen", sf::Color(144, 238, 144)},
    {"LightPink", sf::Color(255, 182, 193)},
    {"LightYellow", sf::Color(255, 255, 224)},
    {"LightPurple", sf::Color(221, 160, 221)},
    {"LightOrange", sf::Color(255, 160, 122)},
    {"LightCyan", sf::Color(224, 255, 255)},
    {"LightBrown", sf::Color(210, 105, 30)},
    {"DarkBlue", sf::Color(0, 0, 139)},
    {"DarkGreen", sf::Color(0, 100, 0)},
    {"DarkPink", sf::Color(199, 21, 133)},
    {"DarkYellow", sf::Color(218, 165, 32)},
    {"DarkPurple", sf::Color(75, 0, 130)},
    {"DarkOrange", sf::Color(255, 140, 0)},
    {"DarkCyan", sf::Color(0, 139, 139)},
    {"DarkBrown", sf::Color(139, 69, 19)}
};

#endif // COLORMAP_HPP