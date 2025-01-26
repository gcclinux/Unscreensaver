#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>

void saveConfig(const std::string& folderPath, int width, int height, int quantify, int duration, const std::string& accessKey, const std::string& secretKey, const std::string& searchPattern) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "caching_folder" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "path" << YAML::Value << folderPath;
    out << YAML::EndMap;

    out << YAML::Key << "image_processing" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "width" << YAML::Value << width;
    out << YAML::Key << "height" << YAML::Value << height;
    out << YAML::Key << "quantify" << YAML::Value << quantify;
    out << YAML::Key << "duration" << YAML::Value << duration;
    out << YAML::EndMap;

    out << YAML::Key << "unsplash" << YAML::BeginMap;
    out << YAML::Key << "access_key" << YAML::Value << accessKey;
    out << YAML::Key << "secret_key" << YAML::Value << secretKey;
    out << YAML::Key << "search_pattern" << YAML::Value << searchPattern;
    out << YAML::EndMap;

    out << YAML::EndMap;

    std::ofstream fout("config.yml");
    fout << out.c_str();
}

void runSetup() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Setup Config");
    window.setKeyRepeatEnabled(true);

    sf::Font font;
    if (!font.loadFromFile("./fonts/LiberationMono-Regular.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }

    // Load existing config
    YAML::Node config;
    try {
        config = YAML::LoadFile("config.yml");
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
    }

    // Input fields
    std::vector<sf::String> inputs = {
        config["caching_folder"]["path"].as<std::string>(""),
        std::to_string(config["image_processing"]["width"].as<int>(512)),
        std::to_string(config["image_processing"]["height"].as<int>(384)),
        std::to_string(config["image_processing"]["quantify"].as<int>(50)),
        std::to_string(config["image_processing"]["duration"].as<int>(5)),
        config["unsplash"]["access_key"].as<std::string>(""),
        config["unsplash"]["secret_key"].as<std::string>(""),
        config["unsplash"]["search_pattern"].as<std::string>("")
    };

    // Labels
    std::vector<std::string> labels = {
        "Folder Path:", "Width:", "Height:", "Quantify:",
        "Duration:", "Access Key:", "Secret Key:", "Search Pattern:"
    };

    std::vector<sf::Text> labelTexts;
    std::vector<sf::Text> inputTexts;
    std::vector<sf::RectangleShape> inputBoxes;

    // Create visual elements
    for (size_t i = 0; i < labels.size(); i++) {
        // Labels
        sf::Text label(labels[i], font, 18);
        label.setPosition(10, 50 + i * 40);
        label.setFillColor(sf::Color::Black);
        labelTexts.push_back(label);

        // Input boxes
        sf::RectangleShape box(sf::Vector2f(550, 30));
        box.setPosition(200, 45 + i * 40);
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(1);
        inputBoxes.push_back(box);

        // Input texts
        sf::Text input(inputs[i], font, 18);
        input.setPosition(205, 50 + i * 40);
        input.setFillColor(sf::Color::Black);
        inputTexts.push_back(input);
    }

    int activeField = -1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (size_t i = 0; i < inputBoxes.size(); i++) {
                    if (inputBoxes[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        activeField = i;
                        inputBoxes[i].setOutlineColor(sf::Color::Blue);
                    } else {
                        inputBoxes[i].setOutlineColor(sf::Color::Black);
                    }
                }
            }

            // Handle text input
            if (event.type == sf::Event::TextEntered && activeField != -1) {
                if (event.text.unicode == '\b') {
                    if (!inputs[activeField].isEmpty()) {
                        inputs[activeField].erase(inputs[activeField].getSize() - 1);
                    }
                }
                else if (event.text.unicode < 128) {
                    inputs[activeField] += event.text.unicode;
                }
                inputTexts[activeField].setString(inputs[activeField]);
            }
        }

        window.clear(sf::Color::White);

        // Draw everything
        for (size_t i = 0; i < labels.size(); i++) {
            window.draw(inputBoxes[i]);
            window.draw(labelTexts[i]);
            window.draw(inputTexts[i]);
        }

        window.display();
    }

    // Save config when window is closed
    if (!inputs[0].isEmpty()) {
        try {
            saveConfig(
                inputs[0].toAnsiString(),  // folder path as string
                std::stoi(inputs[1].toAnsiString()),  // width
                std::stoi(inputs[2].toAnsiString()),  // height
                std::stoi(inputs[3].toAnsiString()),  // quantify
                std::stoi(inputs[4].toAnsiString()),  // duration
                inputs[5].toAnsiString(),  // access key
                inputs[6].toAnsiString(),  // secret key
                inputs[7].toAnsiString()   // search pattern
            );
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid number format in input fields\n";
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: Number out of range in input fields\n";
        }
    }
}