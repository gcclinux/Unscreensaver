#include "headers/checkconfig.hpp"
#include <iostream>
#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

bool isConfigFileExists() {
    if (!fs::exists("config.yml")) {
        std::cerr << "WARNING!!! Config file not found." << std::endl;
        std::cerr << "RUN: ./unscreensaver --setup" << std::endl;
        return false;
    }
    return true;
}

bool isConfigValid(UnScreenSaverConfig& config) {
    if (!fs::exists("config.yml")) {
        std::cerr << "WARNING!!! Config file not found." << std::endl;
        std::cerr << "RUN: ./unscreensaver --setup" << std::endl;
        return false;
    }

    try {
        YAML::Node yaml = YAML::LoadFile("config.yml");

        if (yaml["image_processing"]) {
            auto imgProc = yaml["image_processing"];
            if (imgProc["quantify"]) config.quantify = imgProc["quantify"].as<int>();
            else {
                std::cerr << "Missing 'image_processing.quantify' in config.yml" << std::endl;
                return false;
            }
            if (imgProc["width"]) config.width = imgProc["width"].as<int>();
            else {
                std::cerr << "Missing 'image_processing.width' in config.yml" << std::endl;
                return false;
            }
            if (imgProc["height"]) config.height = imgProc["height"].as<int>();
            else {
                std::cerr << "Missing 'image_processing.height' in config.yml" << std::endl;
                return false;
            }
            if (imgProc["duration"]) config.duration = imgProc["duration"].as<int>();
            else {
                std::cerr << "Missing 'image_processing.duration' in config.yml" << std::endl;
                return false;
            }
        } else {
            std::cerr << "Missing 'image_processing' section in config.yml" << std::endl;
            return false;
        }

        if (yaml["caching_folder"] && yaml["caching_folder"]["path"]) {
            config.folderPath = yaml["caching_folder"]["path"].as<std::string>();
        } else {
            std::cerr << "Missing 'caching_folder.path' in config.yml" << std::endl;
            return false;
        }

        if (yaml["unsplash"]) {
            auto unsplash = yaml["unsplash"];
            if (unsplash["access_key"]) config.accessKey = unsplash["access_key"].as<std::string>();
            else {
                std::cerr << "Missing 'unsplash.access_key' in config.yml" << std::endl;
                return false;
            }
            if (unsplash["secret_key"]) config.secretKey = unsplash["secret_key"].as<std::string>();
            else {
                std::cerr << "Missing 'unsplash.secret_key' in config.yml" << std::endl;
                return false;
            }
            if (unsplash["search_pattern"]) config.searchPattern = unsplash["search_pattern"].as<std::string>();
            else {
                std::cerr << "Missing 'unsplash.search_pattern' in config.yml" << std::endl;
                return false;
            }
        } else {
            std::cerr << "Missing 'unsplash' section in config.yml" << std::endl;
            return false;
        }
    } catch (const YAML::Exception& e) {
        std::cerr << "Error reading config: " << e.what() << std::endl;
        return false;
    }

    return true;
}