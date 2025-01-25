#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include "help.hpp"
#include "readconfig.hpp"
#include "pictures.hpp" // Include the new header file
#include <unordered_set>

namespace fs = std::filesystem;

sf::Color getColorFromString(const std::string& colorName) {
    auto it = COLOR_MAP.find(colorName);
    return it != COLOR_MAP.end() ? it->second : sf::Color::Black;
}

sf::Color getContrastingColor(const sf::Color& bgColor) {
    if (bgColor == sf::Color::Black) return sf::Color::White;
    if (bgColor == sf::Color::White || bgColor == sf::Color::Yellow) return sf::Color::Black;
    return sf::Color::White;
}

std::string getRandomImagePath(const std::string& folderPath) {
    static std::unordered_set<std::string> shownImages;
    static std::vector<std::string> images;

    // If images vector is empty, populate it with all image paths
    if (images.empty()) {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                images.push_back(entry.path().string());
            }
        }
    }

    // If all images have been shown, reset the shownImages set
    if (shownImages.size() == images.size()) {
        shownImages.clear();
    }

    // Select a random image that has not been shown yet
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::string selectedImage;
    do {
        int randomIndex = std::rand() % images.size();
        selectedImage = images[randomIndex];
    } while (shownImages.find(selectedImage) != shownImages.end());

    // Add the selected image to the shownImages set
    shownImages.insert(selectedImage);

    return selectedImage;
}

sf::RectangleShape createAndPositionRandomImage(const std::string& folderPath, const sf::RenderWindow& window) {
    std::string randomImagePath = getRandomImagePath(folderPath);
    if (!randomImagePath.empty()) {
        sf::RectangleShape pixelImage = createPixelImage(randomImagePath);

        // Set a random position for the pixel image
        float randomX = static_cast<float>(std::rand() % (window.getSize().x - static_cast<int>(pixelImage.getSize().x)));
        float randomY = static_cast<float>(std::rand() % (window.getSize().y - static_cast<int>(pixelImage.getSize().y)));
        pixelImage.setPosition(randomX, randomY);

        return pixelImage;
    }
    return sf::RectangleShape(); // Return an empty shape if no image is found
}

int RenderImage(int argc, char* argv[]) {
    UnScreenSaverConfig imgConfig = getConfigFromYaml();
    sf::Color backgroundColor = sf::Color::Black;
    bool showInfo = false;

    sf::Font font;
    if (!font.loadFromFile("./fonts/LiberationMono-Regular.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(24);

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--help") {
            std::cout << getHelpText();
            return 0;
        }
        if (arg == "--info") {
            showInfo = true;
            backgroundColor = sf::Color::Yellow; //Background Colour
            infoText.setFillColor(sf::Color::Black); // Set font color to black
        }
        if ((arg == "--colour" || arg == "--color") && i + 1 < argc) {
            backgroundColor = getColorFromString(argv[i + 1]);
            i++;
        }
    }

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen | sf::Style::None);

    float padding = 20.0f;
    float lineSpacing = 30.0f;

    std::stringstream ss;
    ss << getHelpText() << "\n"; // Add help information at the top
    ss << "Screen Dimentions       : " << window.getSize().x << "x" << window.getSize().y << "\n"
       << "Total images to show    : " << imgConfig.quantify << "\n"
       << "Images size to display  : " << imgConfig.width << "x" << imgConfig.height << "\n"
       << "Images Folder           : " << imgConfig.folderPath << "\n"
       << "Seconds between images  : " << imgConfig.duration << "\n";
    
    infoText.setString(ss.str());
    infoText.setPosition(window.getSize().x - infoText.getGlobalBounds().width - padding, padding);

    std::vector<sf::RectangleShape> images;
    sf::Clock clock;
    float duration = imgConfig.duration;

    // Construct the path to the images
    std::string picturePath = imgConfig.folderPath + "/" + imgConfig.searchPattern;

    // Add the first image immediately
    sf::RectangleShape initialImage = createAndPositionRandomImage(picturePath, window);
    if (initialImage.getSize().x > 0 && initialImage.getSize().y > 0) {
        images.push_back(initialImage);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed &&
               (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q)) {
                window.close();
                return 0;
            }
        }

        if (clock.getElapsedTime().asSeconds() >= duration) {
            clock.restart();
            if (images.size() < imgConfig.quantify) {
                sf::RectangleShape pixelImage = createAndPositionRandomImage(picturePath, window);
                if (pixelImage.getSize().x > 0 && pixelImage.getSize().y > 0) {
                    images.push_back(pixelImage);
                }
            } else {
                images.clear();
                // Add a single random image immediately after clearing
                sf::RectangleShape pixelImage = createAndPositionRandomImage(picturePath, window);
                if (pixelImage.getSize().x > 0 && pixelImage.getSize().y > 0) {
                    images.push_back(pixelImage);
                }
            }
        }

        window.clear(backgroundColor);
        if (showInfo) {
            infoText.setFillColor(getContrastingColor(backgroundColor));
            window.draw(infoText);
        } else {
            for (const auto& image : images) {
                window.draw(image);
            }
        }
        window.display();
    }

    return 0;
}