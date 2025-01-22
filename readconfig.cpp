#include "readconfig.hpp"
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

std::string getHomeFolder() {
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return std::string(homedir) + "/Pictures";
}

UnScreenSaverConfig getConfigFromYaml() {
    UnScreenSaverConfig config{50, 512, 306, 3, getHomeFolder(), "", "", "mountain"}; // defaults
    try {
        YAML::Node yaml = YAML::LoadFile("config.yml");
        if (yaml["image_processing"]) {
            auto imgProc = yaml["image_processing"];
            if (imgProc["quantify"]) config.quantify = imgProc["quantify"].as<int>();
            if (imgProc["width"]) config.width = imgProc["width"].as<int>();
            if (imgProc["height"]) config.height = imgProc["height"].as<int>();
            if (imgProc["duration"]) config.duration = imgProc["duration"].as<int>();
        }
        if (yaml["caching_folder"] && yaml["caching_folder"]["path"]) {
            config.folderPath = yaml["caching_folder"]["path"].as<std::string>();
        }
        if (yaml["unsplash"]) {
            auto unsplash = yaml["unsplash"];
            if (unsplash["access_key"]) config.accessKey = unsplash["access_key"].as<std::string>();
            if (unsplash["secret_key"]) config.secretKey = unsplash["secret_key"].as<std::string>();
            if (unsplash["search_pattern"]) config.searchPattern = unsplash["search_pattern"].as<std::string>();
        }
    } catch (const YAML::Exception& e) {
        std::cerr << "Error reading config: " << e.what() << std::endl;
    }
    return config;
}